// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Landmark_Base.generated.h"

UCLASS()
class RADARSYSTEM_API ALandmark_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandmark_Base();

protected :
	UPROPERTY(VisibleDefaultsOnly, Category = "Configuration|Components")
	class UBillboardComponent* m_pBillboard;

	UPROPERTY(EditAnywhere, Category = "Configuration")
	UTexture2D* m_pIconOnRadar;

public :
	virtual void OnConstruction(const FTransform& Transform) override;

	/* Get */
	FORCEINLINE class UBillboardComponent* const& GetBillboard() const { return m_pBillboard; }

	FORCEINLINE UTexture2D* const& GetIconOnRadar() const { return m_pIconOnRadar; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
};
