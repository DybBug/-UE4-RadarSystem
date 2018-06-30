// Fill out your copyright notice in the Description page of Project Settings.

#include "Secret_Base.h"
#include "Components/Component_Radar.h"

#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>


// Sets default values
ASecret_Base::ASecret_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pHighlightRange = CreateDefaultSubobject<USphereComponent>(TEXT("HighlightRange"));
	m_pHighlightRange->OnComponentBeginOverlap.AddDynamic(this, &ASecret_Base::_OnComponentBeginOverlap);
	m_pHighlightRange->OnComponentEndOverlap.AddDynamic(this, &ASecret_Base::_OnComponentEndOverlap);
	RootComponent = m_pHighlightRange;

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_pStaticMesh->SetupAttachment(RootComponent);

}

void ASecret_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	m_pHighlightRange->SetSphereRadius(m_HighlightRadius);
}

// Called when the game starts or when spawned
void ASecret_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

/* Binding Functions. */

void ASecret_Base::_OnComponentBeginOverlap(
	UPrimitiveComponent* _pOverlappedComponent, 
	AActor* _pOtherActor, 
	UPrimitiveComponent* _pOtherComp, 
	int32 _OtherBodyIndex, 
	bool _bFromSweep, 
	const FHitResult& _SweepResult)
{
	UActorComponent* pComp = _pOtherActor->GetComponentByClass(UComponent_Radar::StaticClass());
	if (pComp)
	{
		UComponent_Radar* pRadarComp = CastChecked<UComponent_Radar>(pComp);
		if (pRadarComp)
		{
			pRadarComp->OnEnterSecretRange(this);
		}
	}
}

void ASecret_Base::_OnComponentEndOverlap(
	UPrimitiveComponent* _pOverlappedComponent, 
	AActor* _pOtherActor, 
	UPrimitiveComponent* _pOtherComp, 
	int32 _OtherBodyIndex)
{
	UActorComponent* pComp = _pOtherActor->GetComponentByClass(UComponent_Radar::StaticClass());
	if (pComp)
	{
		UComponent_Radar* pRadarComp = CastChecked<UComponent_Radar>(pComp);
		if (pRadarComp)
		{
			pRadarComp->OnLeaveSecretRange(this);
		}
	}
}


