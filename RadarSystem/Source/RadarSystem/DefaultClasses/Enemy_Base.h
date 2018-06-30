// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <AIController.h>

#include "Enemy_Base.generated.h"

UCLASS()
class RADARSYSTEM_API AEnemy_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_Base();

protected :
	UPROPERTY(VisibleAnywhere, Category = "Configuration|Components")
	class USphereComponent* m_pPatrolSphere;
	UPROPERTY(VisibleAnywhere, Category = "Configuration|Components")
	class USphereComponent* m_pRadarSphere;

	UPROPERTY(EditAnywhere, Category = "Configuration|Value")
	float m_PatrolRadius = 600.f;
	UPROPERTY(EditAnywhere, Category = "Configuration|Value")
	float m_MovementSpeed = 120.f;
	UPROPERTY(EditAnywhere, Category = "Configuration|Value")
	float m_OnRadarRadius = 1000.f;

	FVector m_StartLocation;

	FTimerHandle m_hPatrolTimer;
	FTimerHandle m_hRadarTimer;

	bool m_bCurrentlyMoving;
	bool m_bOnRadar;

	class UComponent_Radar* m_pPlayerRadar;

public :
	virtual void OnConstruction(const FTransform& _Trans) override;

	void InitializeEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void _OnStartMoving();

	UFUNCTION()
	void _StartPatrolling();

	UFUNCTION()
	void _OnStopMoving();

	UFUNCTION()
	void _OnSuccessMoveTo(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void _UpdateOnRadar();

	UFUNCTION()
	void _OnComponentBeginOverlap(
		UPrimitiveComponent* _pOverlappedComponent, 
		AActor* _pOtherActor, 
		UPrimitiveComponent* _pOtherComp, 
		int32 _OtherBodyIndex, 
		bool _bFromSweep, 
		const FHitResult& _SweepResult);
	
	UFUNCTION()
	void _OnComponentEndOverlap(
		UPrimitiveComponent* _pOverlappedComponent,
		AActor* _pOtherActor,
		UPrimitiveComponent* _pOtherComp, 
		int32 _OtherBodyIndex);
	

	void _StartUpdateTimer();
	
};
