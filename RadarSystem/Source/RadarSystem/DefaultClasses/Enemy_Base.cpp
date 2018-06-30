// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Base.h"
#include "Components/Component_Radar.h"
#include "RadarSystemCharacter.h"

#include <Components/SphereComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <AIController.h>
#include <AI/Navigation/NavigationSystem.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/World.h>
#include <TimerManager.h>


// Sets default values
AEnemy_Base::AEnemy_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pPatrolSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PatrolSphere"));
	m_pPatrolSphere->SetupAttachment(RootComponent);

	m_pRadarSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RadarSphere"));
	m_pRadarSphere->ShapeColor = FColor(0xFFEAEA1D);
	m_pRadarSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Base::_OnComponentBeginOverlap);
	m_pRadarSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy_Base::_OnComponentEndOverlap);
	m_pRadarSphere->SetupAttachment(RootComponent);
}

void AEnemy_Base::OnConstruction(const FTransform& _Trans)
{
	Super::OnConstruction(_Trans);

	m_pPatrolSphere->SetSphereRadius(m_PatrolRadius);
	m_pRadarSphere->SetSphereRadius(m_OnRadarRadius);

	GetCharacterMovement()->MaxWalkSpeed = m_MovementSpeed;
}

void AEnemy_Base::InitializeEnemy()
{
	TSet<AActor*> Actors;
	m_pRadarSphere->GetOverlappingActors(Actors, ARadarSystemCharacter::StaticClass());
	for (auto& Iter : Actors)
	{
		UActorComponent* pComp = Iter->GetComponentByClass(UComponent_Radar::StaticClass());
		if (pComp)
		{
			UComponent_Radar* pRadar = CastChecked<UComponent_Radar>(pComp);
			pRadar->AddEnemyToRader(this);
			m_bOnRadar = true;
			m_pPlayerRadar = pRadar;
		}
	}
}

// Called when the game starts or when spawned
void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	AAIController* pAIController = CastChecked<AAIController>(GetController());
	pAIController->ReceiveMoveCompleted.AddDynamic(this, &AEnemy_Base::_OnSuccessMoveTo);

	m_StartLocation = GetActorLocation();

	_StartPatrolling();
}

void AEnemy_Base::_OnStartMoving()
{
	m_bCurrentlyMoving = true;

	if (m_bOnRadar)
	{
		if (m_pPlayerRadar)
		{
			_UpdateOnRadar();
			_StartUpdateTimer();
		}
	}
}

void AEnemy_Base::_StartPatrolling()
{
	_OnStartMoving();
	
	AAIController* pAIController = CastChecked<AAIController>(GetController());
	FAIMoveRequest Request;		
	Request.SetGoalLocation(UNavigationSystem::GetRandomReachablePointInRadius(GetWorld(), m_StartLocation, m_PatrolRadius));
	Request.SetAcceptanceRadius(5.0);
	pAIController->MoveTo(Request);		
}

void AEnemy_Base::_OnStopMoving()
{
	m_bCurrentlyMoving = false;
	UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, m_hRadarTimer);

	if (m_bOnRadar)
	{
		if (m_pPlayerRadar)
		{
			_UpdateOnRadar();
		}
	}
}

void AEnemy_Base::_OnSuccessMoveTo(FAIRequestID _RequestID, EPathFollowingResult::Type _Result)
{
	_OnStopMoving();

	float DelayTime = FMath::RandRange(1.f, 3.f);

	GetWorldTimerManager().SetTimer(m_hPatrolTimer, [&]() {
		_StartPatrolling();
		UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, m_hPatrolTimer);
	}, 1.f, false, DelayTime);
	
	//FLatentActionInfo Latent;
	//Latent.CallbackTarget = this;
	//Latent.ExecutionFunction = TEXT("_StartPatrolling");
	//Latent.UUID = GetUniqueID();
	//UKismetSystemLibrary::Delay(this, DelayTime, Latent);

}

void AEnemy_Base::_UpdateOnRadar()
{
	m_pPlayerRadar->OnEnemyMoved(this);
}

void AEnemy_Base::_OnComponentBeginOverlap(
	UPrimitiveComponent* _pOverlappedComponent, 
	AActor* _pOtherActor, 
	UPrimitiveComponent* _pOtherComp, 
	int32 _OtherBodyIndex,
	bool _bFromSweep, 
	const FHitResult& _SweepResult)
{
	if (!m_bOnRadar)
	{
		UActorComponent* pComp = _pOtherActor->GetComponentByClass(UComponent_Radar::StaticClass());
		if (pComp)
		{
			UComponent_Radar* pRadar = CastChecked<UComponent_Radar>(pComp);
			if (pRadar)
			{
				pRadar->AddEnemyToRader(this);
				m_bOnRadar = true;
				m_pPlayerRadar = pRadar;

				if (m_bCurrentlyMoving)
				{
					_StartUpdateTimer();
				}
			}
		}
	}
}

void AEnemy_Base::_OnComponentEndOverlap(
	UPrimitiveComponent* _pOverlappedComponent, 
	AActor* _pOtherActor, 
	UPrimitiveComponent* _pOtherComp, 
	int32 _OtherBodyIndex)
{
	if (m_bOnRadar)
	{
		UActorComponent* pComp = _pOtherActor->GetComponentByClass(UComponent_Radar::StaticClass());
		if (pComp)
		{
			UComponent_Radar* pRadar = CastChecked<UComponent_Radar>(pComp);
			if (pRadar)
			{
				m_bOnRadar = false;
				UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, m_hRadarTimer);
				pRadar->RemoveEnemyFromRadar(this);

				m_pPlayerRadar = nullptr;
			}
		}
	}
}

void AEnemy_Base::_StartUpdateTimer()
{
	GetWorldTimerManager().SetTimer(m_hRadarTimer, this, &AEnemy_Base::_UpdateOnRadar, 0.02f, true);
}

