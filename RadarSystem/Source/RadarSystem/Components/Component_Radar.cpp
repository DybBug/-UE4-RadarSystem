// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_Radar.h"
#include "Widgets/Widget_Main.h"
#include "Widgets/Widget_Radar.h"
#include "Widgets/Widget_Direction.h"
#include "Widgets/Widget_Marker.h"
#include "Widgets/Widget_Landmark.h"
#include "Widgets/Widget_EnemyMarker.h"
#include "DefaultClasses/Landmark_Base.h"
#include "DefaultClasses/Secret_Base.h"
#include "DefaultClasses/Enemy_Base.h"
#include "RadarSystemCharacter.h"

#include <TimerManager.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Animation/WidgetAnimation.h>
#include <Camera/CameraComponent.h>


// Sets default values for this component's properties
UComponent_Radar::UComponent_Radar()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	// #. 방향 정보 기본(동, 서, 남, 북) 설정.
	m_Directions.Emplace(TEXT("N"), 0);   // 북
	m_Directions.Emplace(TEXT("E"), 90);  // 동
	m_Directions.Emplace(TEXT("S"), 180); // 남
	m_Directions.Emplace(TEXT("W"), -90); // 서
}

void UComponent_Radar::InitializeRadar(class ARadarSystemCharacter* _pPlayer)
{
	m_pPlayer = _pPlayer;
	m_pPlayerCamera = m_pPlayer->GetFollowCamera();

	UClass* pWB_MainClass = LoadClass<UWidget_Main>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_Main.WB_Main_C'"));
	if (pWB_MainClass)
	{
		m_pWidget_Main  = CreateWidget<UWidget_Main>(GetWorld(), pWB_MainClass);
		m_pWidget_Radar = m_pWidget_Main->GetWidget_Radar();

		// #. 레이더에 기본(동, 서, 남, 북) 방향 정보 추가.
		for (auto& Iter : m_Directions)
		{
			m_pWidget_Radar->AddDirection(Iter);
		}

		// #. 기본 표식들 추가하기.
		for (auto& Iter : m_DefaultQuestMarkers)
		{
			AddMarkerToRader(Iter);
		}

		// #. 랜드마크 추가 하기.
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandmark_Base::StaticClass(), Actors);
		for (auto& Iter : Actors)
		{
			ALandmark_Base* LandmarkActor = CastChecked<ALandmark_Base>(Iter);
			AddLandmarkToRader(LandmarkActor);
		}

		// #. 방향, 표식, 랜드마크 업데이트.
		OnBecomeVisible();

		_OnRadarInitialized();

		m_pWidget_Main->AddToViewport();
	}
}


void UComponent_Radar::OnBecomeVisible()
{
	UpdateDirectionWidgets();

	UpdateMarkerDistances();
	UpdateMarkerPositions();
	UpdateMarkerElevation();

	UpdateLandmarkPositions();

	UpdateEnemyMarkerDistances();
	UpdateEnemyMarkerPositions();

}

void UComponent_Radar::ToggleVisibility(bool _bAnimate)
{
	_SetVisibility(!m_bVisible, _bAnimate);
}

void UComponent_Radar::UpdateDirectionWidgets()
{
	TArray<UWidget_Direction*> pDirectionWidgets = m_pWidget_Radar->GetDirectionWidgets();
	for(int Index = 0; Index <  pDirectionWidgets.Num(); ++Index)
	{	
		bool bInRadarSight;
		FVector2D Trans;

		FRotator PlayerCamRot = m_pPlayerCamera->GetComponentRotation();
		FRotator TargetRot = FRotator(0.f, m_Directions[Index].WorldRotation, 0.f);

		_RotationsToTranslation(PlayerCamRot, TargetRot, &Trans, nullptr, &bInRadarSight);

		if (bInRadarSight)
		{
			FWidgetTransform RenderTrans;
			RenderTrans.Translation = Trans;
			pDirectionWidgets[Index]->SetRenderTransform(RenderTrans);
			if (pDirectionWidgets[Index])
			{
				pDirectionWidgets[Index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
		}
		else
		{
			if (pDirectionWidgets[Index])
			{
				//UpdateDirectionWidgets();
				pDirectionWidgets[Index]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UComponent_Radar::OnPlayerTurned()
{
	if (m_bVisible)
	{
		UpdateDirectionWidgets();
		UpdateMarkerPositions();
		UpdateMarkerElevation();
		UpdateLandmarkPositions();
		UpdateEnemyMarkerPositions();
	}
}

void UComponent_Radar::OnPlayerMoved()
{
	if (m_bVisible)
	{	
		UpdateMarkerDistances();
		UpdateMarkerPositions();
		UpdateLandmarkPositions();
		UpdateEnemyMarkerDistances();
		UpdateEnemyMarkerPositions();
	}
}

void UComponent_Radar::OnEnemyMoved(AEnemy_Base* _pEnemy)
{
	UpdateMarkerDistanceForEnemy(_pEnemy);
	UpdateMarkerPositionForEnemy(_pEnemy);
}

void UComponent_Radar::AddMarkerToRader(FMarkerInfo _Info)
{
	m_QuestMarkers.Emplace(_Info);
	if (m_pWidget_Radar)
	{
		m_pWidget_Radar->AddMarker(_Info.Type);
	}
}

void UComponent_Radar::AddLandmarkToRader(ALandmark_Base* _pLandmark)
{
	if (!m_Landmarks.Contains(_pLandmark))
	{
		m_Landmarks.Emplace(_pLandmark);
		m_pWidget_Radar->AddLandmark(_pLandmark->GetIconOnRadar());
	}
}

void UComponent_Radar::AddEnemyToRader(AEnemy_Base * _pEnemy)
{
	if (m_pWidget_Radar)
	{
		if (!m_EnemiesOnRadar.Contains(_pEnemy))
		{
			m_EnemiesOnRadar.Emplace(_pEnemy);

			m_pWidget_Radar->AddEnemyMarker();

			UpdateMarkerDistanceForEnemy(_pEnemy);
			UpdateMarkerPositionForEnemy(_pEnemy);
		}
	}
}

void UComponent_Radar::UpdateMarkerDistances()
{
	const TArray<UWidget_Marker*>& MarkerWidgets = m_pWidget_Radar->GetMarkerWidgets();
	for(int i = 0; i < MarkerWidgets.Num(); ++i)
	{
		FVector2D MarkerLocation = FVector2D(m_QuestMarkers [i].Location.X, m_QuestMarkers [i].Location.Y);
		FVector2D PlayerLocation = FVector2D(m_pPlayer->GetActorLocation().X, m_pPlayer->GetActorLocation().Y);
		FVector2D DistanceVec    = MarkerLocation - PlayerLocation;
		float     Distance       = (DistanceVec.Size() / m_UnitsPerMeter);

		// #. 소수점 이하를 반올림하여 정수로
		//     0.5.. -> 1, 
		//     0.4.. -> 0		
		MarkerWidgets[i]->UpdateDistance(FMath::RoundToInt(Distance)); 
	}
}

void UComponent_Radar::UpdateMarkerPositions()
{
	const TArray<UWidget_Marker*>& MarkerWidgets = m_pWidget_Radar->GetMarkerWidgets();
	for(int i = 0; i < MarkerWidgets.Num(); ++i)
	{
		FVector MarkerLocation = m_QuestMarkers[i].Location;
		FVector PlayerLocation = m_pPlayer->GetActorLocation();

		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, MarkerLocation);
		FRotator PlayerCameraRot = m_pPlayerCamera->GetComponentRotation();

		bool bInRadarSight;
		bool bClockwise;
		FVector2D Trans;
		_RotationsToTranslation(PlayerCameraRot, LookAtRot, &Trans, &bClockwise, &bInRadarSight);

		if (m_bHideOutOfSightMarkers)
		{
			if (bInRadarSight)
			{
				FWidgetTransform WidgetTrans;
				WidgetTrans.Translation = Trans;
				MarkerWidgets[i]->SetRenderTransform(WidgetTrans);

				if (!MarkerWidgets[i]->IsVisible())
				{
					MarkerWidgets[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}
			else
			{
				if (MarkerWidgets[i]->IsVisible())
				{
					MarkerWidgets[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
		else
		{
			if (bInRadarSight)
			{
				FWidgetTransform WidgetTrans;
				WidgetTrans.Translation = Trans;
				MarkerWidgets[i]->SetRenderTransform(WidgetTrans);
			}
			else
			{
				FWidgetTransform WidgetTrans;
				WidgetTrans.Translation.X = bClockwise ? m_MaxWidgetTranslation : -m_MaxWidgetTranslation;
				WidgetTrans.Translation.Y = 0.f;
				MarkerWidgets[i]->SetRenderTransform(WidgetTrans);
			}			
		}
	}
}

void UComponent_Radar::UpdateLandmarkPositions()
{
	const TArray<UWidget_Landmark*>& LandmarkWidgets = m_pWidget_Radar->GetLandmarkWidgets();
	for(int i = 0; i < LandmarkWidgets.Num(); ++i)
	{
		FVector LandmarkLocation = m_Landmarks[i]->GetActorLocation();
		FVector PlayerLocation = m_pPlayer->GetActorLocation();

		FRotator LookAtRot       = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, LandmarkLocation);
		FRotator PlayerCameraRot = m_pPlayerCamera->GetComponentRotation();

		bool bInRadarSight;
		FVector2D Trans;
		_RotationsToTranslation(PlayerCameraRot, LookAtRot, &Trans, nullptr, &bInRadarSight);

		if (bInRadarSight)
		{
			FWidgetTransform WidgetTrans;
			WidgetTrans.Translation = Trans;
			LandmarkWidgets[i]->SetRenderTransform(WidgetTrans);

			if (!LandmarkWidgets[i]->IsVisible())
			{
				LandmarkWidgets[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}		
		}
		else
		{
			if (LandmarkWidgets[i]->IsVisible())
			{
				LandmarkWidgets[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}		
	}
}

void UComponent_Radar::UpdateMarkerDistanceForEnemy(AEnemy_Base* _pEnemy)
{
	int Index = m_EnemiesOnRadar.Find(_pEnemy);
	if (Index >= 0)
	{
		if (m_pWidget_Radar->GetEnemyMarkerWidgets()[Index])
		{
			int Distance = DistanceToPlayer(_pEnemy);
			m_pWidget_Radar->GetEnemyMarkerWidgets()[Index]->UpdateDistance(Distance);
		}
	}
}

void UComponent_Radar::UpdateMarkerPositionForEnemy(AEnemy_Base * _pEnemy)
{
	int Index = m_EnemiesOnRadar.Find(_pEnemy);
	if (Index >= 0)
	{
		UWidget_EnemyMarker* pEnemyMarkerWidget = m_pWidget_Radar->GetEnemyMarkerWidgets()[Index];
		if (pEnemyMarkerWidget)
		{
			FVector EnemyLocation = _pEnemy->GetActorLocation();
			FVector PlayerLocation = m_pPlayer->GetActorLocation();

			FRotator LookAtRot       = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, EnemyLocation);
			FRotator PlayerCameraRot = m_pPlayerCamera->GetComponentRotation();

			bool bInRadarSight;
			FVector2D Trans;
			_RotationsToTranslation(PlayerCameraRot, LookAtRot, &Trans, nullptr, &bInRadarSight);

			if (bInRadarSight)
			{
				FWidgetTransform WidgetTrans;
				WidgetTrans.Translation = Trans;
				pEnemyMarkerWidget->SetRenderTransform(WidgetTrans);

				if (!pEnemyMarkerWidget->IsVisible())
				{
					pEnemyMarkerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}		
			}
			else
			{
				if (pEnemyMarkerWidget->IsVisible())
				{
					pEnemyMarkerWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
}

void UComponent_Radar::UpdateMarkerElevation()
{
	TArray<UWidget_Marker*> MarkerWidgets = m_pWidget_Radar->GetMarkerWidgets();
	for (int i = 0; i < MarkerWidgets.Num(); ++i)
	{
		float PlayerHeight = m_pPlayer->GetActorLocation().Z;
		float MarkerHeight = m_QuestMarkers[i].Location.Z;
		float Direction = MarkerHeight - PlayerHeight;

		if (FMath::Abs(Direction) <= m_DefaultElevationRange)
		{
			Direction = 0.f;
		}

		MarkerWidgets[i]->SetArrowDirection(Direction / FMath::Abs(Direction));
	}
}

void UComponent_Radar::UpdateEnemyMarkerDistances()
{
	for (auto& Iter : m_EnemiesOnRadar)
	{
		UpdateMarkerDistanceForEnemy(Iter);
	}
}

void UComponent_Radar::UpdateEnemyMarkerPositions()
{
	for (auto& Iter : m_EnemiesOnRadar)
	{
		UpdateMarkerPositionForEnemy(Iter);
	}
}

bool UComponent_Radar::RemoveMarkerAtIndex(int _Index)
{
	if (UKismetMathLibrary::InRange_IntInt(_Index, 0, m_QuestMarkers.Num() - 1) &&
		m_QuestMarkers.Num() > 0)
	{
		return _RemoveQuestMarker(m_QuestMarkers[_Index]);
	}
	return false;
}

void UComponent_Radar::RemoveEnemyFromRadar(AEnemy_Base* _pEnemy)
{
	int Index = m_EnemiesOnRadar.Find(_pEnemy);
	if (Index != -1)
	{
		m_EnemiesOnRadar.RemoveAt(Index);
		m_pWidget_Radar->RemoveEnemyMarker(Index);
	}
}

void UComponent_Radar::OnEnterSecretRange(ASecret_Base* _pSecret)
{
	if (!m_SecretsInRange.Contains(_pSecret))
	{
		m_SecretsInRange.Emplace(_pSecret);

		if (m_SecretsInRange.Num() == 1)
		{
			m_pWidget_Radar->HighlightRadar(true);
		}
	}
}

void UComponent_Radar::OnLeaveSecretRange(ASecret_Base * _pSecret)
{
	if (m_SecretsInRange.Contains(_pSecret))
	{
		m_SecretsInRange.Remove(_pSecret);

		if (m_SecretsInRange.Num() == 0)
		{
			m_pWidget_Radar->HighlightRadar(false);
		}
	}
}

int UComponent_Radar::DistanceToPlayer(AActor* _pActor)
{
	FVector2D ActorLocation(_pActor->GetActorLocation().X, _pActor->GetActorLocation().Y);
	FVector2D PlayerLocation(m_pPlayer->GetActorLocation().X, m_pPlayer->GetActorLocation().Y);

	FVector2D DirectionVec = ActorLocation - PlayerLocation;
	float     Direction    = DirectionVec.Size();

	return	FMath::RoundToInt(Direction / m_UnitsPerMeter);
}

// Called when the game starts
void UComponent_Radar::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UComponent_Radar::_OnRadarInitialized()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy_Base::StaticClass(), Actors);

	for (auto& Iter : Actors)
	{
		CastChecked<AEnemy_Base>(Iter)->InitializeEnemy();
	}
}

void UComponent_Radar::_SetVisibility(bool _bVisible, bool _bAnimate)
{
	if ((m_bVisible != _bVisible) && m_bCanChangeVisibility)
	{
		m_bCanChangeVisibility = false;
		m_bVisible = _bVisible;

		if (_bAnimate)
		{
			if (m_bVisible)
			{
				OnBecomeVisible();

				m_pWidget_Radar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				m_pWidget_Radar->PlayAnimation(m_pWidget_Radar->GetBlendOut(), 0.f, 1, EUMGSequencePlayMode::Reverse);

				FTimerHandle hTimer;
				GetWorld()->GetTimerManager().SetTimer(hTimer, [&]() {
					m_bCanChangeVisibility = true;
					UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), hTimer);
				}, 1, false, m_pWidget_Radar->GetBlendOut()->GetEndTime());

			}
			else
			{				
				m_pWidget_Radar->PlayAnimation(m_pWidget_Radar->GetBlendOut(), 0.f, 1, EUMGSequencePlayMode::Forward);

				FTimerHandle hTimer;
				GetWorld()->GetTimerManager().SetTimer(hTimer, [&]() {
					m_pWidget_Radar->SetVisibility(ESlateVisibility::Hidden);
					m_bCanChangeVisibility = true;
					UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(GetWorld(), hTimer);
				}, 1, false, m_pWidget_Radar->GetBlendOut()->GetEndTime());
			}
		}
		else
		{
			if (m_bVisible)
			{
				OnBecomeVisible();
			}
			ESlateVisibility Visibility = m_bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
			m_pWidget_Radar->SetVisibility(Visibility);
			m_bCanChangeVisibility = true;
		}
	}
}

float UComponent_Radar::_RotationToCircleDegrees(float _Value)
{

	// #. 각도가 음수이면 양수방향의 각도로 바꿔 준다.(최대값 360도)
	//    -90도 == 270도.
	// _Value = (_Value < 0.f) ? (_Value + 360.f) % 360.f : _Value;
	//
	_Value = (_Value < 0.f) ? (_Value + 360.f) : _Value;
	_Value =  UKismetMathLibrary::GenericPercent_FloatFloat(_Value, 360.f);

	return _Value;
}

float UComponent_Radar::_GetDeltaClockwise(float _A, float _B, bool _bClockwise)
{
	_A = UKismetMathLibrary::GenericPercent_FloatFloat(_A, 360.f); // _A = _A % 360.f;
	_B = UKismetMathLibrary::GenericPercent_FloatFloat(_B, 360.f); // _B = _B % 360.f;

	// #. 시계 방향.
	if (_bClockwise) 
	{		
		if (_A > _B)
		{
			return 360.f - (_A - _B);
		}
		else if (_A < _B)
		{
			return _B - _A;
		}
	}
	// #. 반시계 방향.
	else 
	{
		if (_A > _B)
		{
			return _A - _B;
		}
		else if (_A < _B)
		{
			return 360.f - (_B - _A); 
		}
	}

	return 0.0f;
}

void UComponent_Radar::_RotationsToTranslation(const FRotator& _RotationA, const FRotator& _RotationB, FVector2D* out_Translation, bool* out_bClockwise, bool* out_bInRadarSight)
{
	float RotA = _RotationToCircleDegrees(_RotationA.Yaw);
	float RotB = _RotationToCircleDegrees(_RotationB.Yaw);

	float Clockwise        = _GetDeltaClockwise(RotA, RotB, true);  // A, B 사이의 시계 방향으로의 변화량.
	float CounterClockwise = _GetDeltaClockwise(RotA, RotB, false); // A, B 사이의 반시계 방향으로의 변화량.

	float FOV = m_pPlayerCamera->FieldOfView;

	if (Clockwise > CounterClockwise)
	{	
		if ((CounterClockwise / FOV) <= 1.f)
		{
			float Value = FMath::Lerp<float>(0.f, m_MaxWidgetTranslation, CounterClockwise / FOV);

			if(out_Translation)   *out_Translation   = FVector2D(-Value, 0.f);
			if(out_bClockwise)    *out_bClockwise    = false;
			if(out_bInRadarSight) *out_bInRadarSight = true;
			return;
		}
		else
		{
			if (out_Translation)   *out_Translation   = FVector2D(0.f, 0.f);
			if (out_bClockwise)    *out_bClockwise    = false;
			if (out_bInRadarSight) *out_bInRadarSight = false;
			return;
		}
	}
	else
	{
		if ((Clockwise / FOV) <= 1.f)
		{
			float Value = FMath::Lerp<float>(0.f, m_MaxWidgetTranslation, Clockwise / FOV);

			if (out_Translation)   *out_Translation   = FVector2D(Value, 0.f);
			if (out_bClockwise)    *out_bClockwise    = true;
			if (out_bInRadarSight) *out_bInRadarSight = true;
			return;
		}
		else
		{
			if (out_Translation)   *out_Translation   = FVector2D(0.f, 0.f);
			if (out_bClockwise)    *out_bClockwise    = true;
			if (out_bInRadarSight) *out_bInRadarSight = false;
			return;
		}
	}

	// #. 디폴트 리턴값.
	if (out_Translation)   *out_Translation   = FVector2D(0.f, 0.f);
	if (out_bClockwise)    *out_bClockwise    = false;
	if (out_bInRadarSight) *out_bInRadarSight = false;
}

bool UComponent_Radar::_RemoveQuestMarker(FMarkerInfo _Info)
{
	int Index = m_QuestMarkers.Find(_Info);
	if (Index != -1)
	{
		m_QuestMarkers.RemoveAt(Index);

		m_pWidget_Radar->RemoveMarker(Index);
		return true;
	}	
	return false;
}

