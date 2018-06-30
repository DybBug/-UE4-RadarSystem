// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Secret_Base.generated.h"

UCLASS()
class RADARSYSTEM_API ASecret_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecret_Base();

protected :
	UPROPERTY(VisibleAnywhere, Category="Configuration|Components")
	class UStaticMeshComponent* m_pStaticMesh;
	UPROPERTY(VisibleAnywhere, Category="Configuration|Components")
	class USphereComponent* m_pHighlightRange;

	UPROPERTY(EditAnywhere, Category="Configuration|Value")
	float m_HighlightRadius = 120.f;

public :
	virtual void OnConstruction(const FTransform& Transform) override;
	
	/* Get */
	FORCEINLINE class UStaticMeshComponent* const& GetStaticMesh()     const { return m_pStaticMesh; }
	FORCEINLINE class USphereComponent*     const& GetHighlightRange() const { return m_pHighlightRange; }

	/* Set */

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected :
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
	
};
