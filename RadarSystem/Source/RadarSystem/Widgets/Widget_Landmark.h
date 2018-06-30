// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Landmark.generated.h"

/**
 * 
 */
UCLASS()
class RADARSYSTEM_API UWidget_Landmark : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	UPROPERTY()
	class UImage* m_pIcon;

	UPROPERTY(EditAnywhere, Category = "Configuration")
	UTexture2D* m_pLandmarkIcon;

public :
	void InitializeWidget(UTexture2D* const& _pIcon);

	/* Get */
	FORCEINLINE class UImage* const& GetIcon() const { return m_pIcon; }
	
protected :
	virtual void NativePreConstruct() override;
};
