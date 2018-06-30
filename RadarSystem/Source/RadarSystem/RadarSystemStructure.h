// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RadarSystemEnums.h"
#include "RadarSystemStructure.generated.h"


USTRUCT(BlueprintType)
struct FDirectionInfo
{
	GENERATED_BODY()

public :
	FDirectionInfo() {};
	FDirectionInfo(const FName& _Name, int _WorldRot)
		: Name(_Name), WorldRotation(_WorldRot) {};

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDirectionInfo")
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FDirectionInfo")
	int WorldRotation;
};

USTRUCT(BlueprintType)
struct FMarkerInfo
{
	GENERATED_BODY()

public :
	FMarkerInfo() {};
	FMarkerInfo(const EMarkerTypes& _Type, const FVector& _Location)
		: Type(_Type), Location(_Location) {};

	bool operator==(const FMarkerInfo& _Ref) const
	{
		return ((this->Type == _Ref.Type) && (this->Location == _Ref.Location));	
	}

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMarkerInfo")
	EMarkerTypes Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMarkerInfo")
	FVector Location;
};

