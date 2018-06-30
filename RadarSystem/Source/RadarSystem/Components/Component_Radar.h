// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RadarSystemStructure.h"
#include "Component_Radar.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RADARSYSTEM_API UComponent_Radar : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_Radar();

protected :
	UPROPERTY()
	class UWidget_Main*  m_pWidget_Main;

	UPROPERTY()
	class UWidget_Radar* m_pWidget_Radar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Defaults")
	TArray<FDirectionInfo> m_Directions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Defaults")
	TArray<FMarkerInfo> m_DefaultQuestMarkers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Defaults")
	float m_UnitsPerMeter = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Defaults")
	bool m_bHideOutOfSightMarkers = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration|Defaults")
	float m_DefaultElevationRange = 150.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration|Markers")
	TArray<FMarkerInfo> m_QuestMarkers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration|Landmarks")
	TArray<class ALandmark_Base*> m_Landmarks;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration|Secrets")
	TArray<class ASecret_Base*> m_SecretsInRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration|Enemies")
	TArray<class AEnemy_Base*> m_EnemiesOnRadar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Configuration")
	float m_MaxWidgetTranslation = 320.f;

	bool m_bVisible = true;
	bool m_bCanChangeVisibility = true;
	class ARadarSystemCharacter* m_pPlayer;
	class UCameraComponent*      m_pPlayerCamera;


public :
	void InitializeRadar(class ARadarSystemCharacter* _pPlayer);
	void OnBecomeVisible();
	void ToggleVisibility(bool _bAnimate);
	void UpdateDirectionWidgets();
	void OnPlayerTurned();
	void OnPlayerMoved();
	void OnEnemyMoved(class AEnemy_Base* _pEnemy);
	void AddMarkerToRader(FMarkerInfo _Info);
	void AddLandmarkToRader(class ALandmark_Base* _pLandmark);
	void AddEnemyToRader(class AEnemy_Base* _pEnemy);
	void UpdateMarkerDistances();
	void UpdateMarkerPositions();
	void UpdateLandmarkPositions();
	void UpdateMarkerDistanceForEnemy(class AEnemy_Base* _pEnemy);
	void UpdateMarkerPositionForEnemy(class AEnemy_Base* _pEnemy);
	void UpdateMarkerElevation();
	void UpdateEnemyMarkerDistances();
	void UpdateEnemyMarkerPositions();
	bool RemoveMarkerAtIndex(int _Index);
	void RemoveEnemyFromRadar(class AEnemy_Base* _pEnemy);
	void OnEnterSecretRange(class ASecret_Base* _pSecret);
	void OnLeaveSecretRange(class ASecret_Base* _pSecret);
	int  DistanceToPlayer(AActor* _pActor);


	/* Get */
	FORCEINLINE class UWidget_Main*  const& GetWidget_Main()  const { return m_pWidget_Main; }
	FORCEINLINE class UWidget_Radar* const& GetWidget_Radar() const { return m_pWidget_Radar; }

	/* Set */

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected :
	void _OnRadarInitialized();

	void  _SetVisibility(bool _bVisible, bool _bAnimate);

	// #. -각을 +각으로 변경한다.(-90 -> 270)
	float _RotationToCircleDegrees(float _Value);

	// #. _A, _B 사이의 시계(true)/반시계(false)방향의 변화량.
	float _GetDeltaClockwise(float _A, float _B, bool _bClockwise);

	void _RotationsToTranslation(
		const FRotator& _RotationA, 
		const FRotator& _RotationB, 
		FVector2D* out_Translation   = nullptr, 
		bool*      out_bClockwise    = nullptr, 
		bool*      out_bInRadarSight = nullptr);

	bool _RemoveQuestMarker(FMarkerInfo _Info);

	
};
