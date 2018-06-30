// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Main.generated.h"

/**
 * 
 */
UCLASS()
class RADARSYSTEM_API UWidget_Main : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	UPROPERTY()
	class UWidget_Radar* m_pWidget_Radar;

public :
	/* Get */
	FORCEINLINE class UWidget_Radar* const& GetWidget_Radar() const { return m_pWidget_Radar; }

	/* Set */

	
	
};
