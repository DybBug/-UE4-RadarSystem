// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "RadarSystemEnums.generated.h"


UENUM(BlueprintType)
enum class EMarkerTypes : uint8
{
	MainQuest UMETA(DisplayName = "MAIN QUEST"),
	SideQuest UMETA(DisplayName = "SIDE QUEST")
};

