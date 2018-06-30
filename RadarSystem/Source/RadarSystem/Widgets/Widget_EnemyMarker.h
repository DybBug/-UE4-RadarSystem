// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_EnemyMarker.generated.h"

/**
 * 
 */
UCLASS()
class RADARSYSTEM_API UWidget_EnemyMarker : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	UPROPERTY()
	class UTextBlock* m_pDistanceText;
	UPROPERTY()
	class UImage* m_pEnemyIcon;

	int m_MaxDisplayedDistance = 999;
	
public :
	void UpdateDistance(int _Distance);

	/* Get */
	FORCEINLINE class UTextBlock* const& GetDistanceText() const { return m_pDistanceText; }
	FORCEINLINE class UImage*     const& GetEnemyIcon() const { return m_pEnemyIcon; }

	
};
