// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadarSystemStructure.h"
#include "Widget_Direction.generated.h"

/**
 * 
 */
UCLASS()
class RADARSYSTEM_API UWidget_Direction : public UUserWidget
{
	GENERATED_BODY()

protected :
	UPROPERTY()
	class UTextBlock* m_pText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
	FDirectionInfo m_DirectionInfo;

public :
	void InitializeWidget(const FDirectionInfo& _DirectionInfo);

	/* Get */
	FORCEINLINE class UTextBlock* const& GetText() const { return m_pText; }

	FORCEINLINE const FDirectionInfo& GetDirectionInfo() const { return m_DirectionInfo; }

protected :
	virtual void NativePreConstruct() override;
	
	
	
};
