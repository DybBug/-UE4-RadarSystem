// Fill out your copyright notice in the Description page of Project Settings.

#include "Landmark_Base.h"

#include <Components/BillboardComponent.h>


// Sets default values
ALandmark_Base::ALandmark_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	m_pBillboard->SetupAttachment(RootComponent);
}

void ALandmark_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_pBillboard->SetSprite(m_pIconOnRadar);
}

// Called when the game starts or when spawned
void ALandmark_Base::BeginPlay()
{
	Super::BeginPlay();
	
}


