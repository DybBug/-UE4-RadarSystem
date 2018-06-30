// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Marker.generated.h"

enum class EMarkerTypes : uint8;

/**
 * 
 */
UCLASS()
class RADARSYSTEM_API UWidget_Marker : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	UPROPERTY()
	class UTextBlock* m_pDistanceText;

	UPROPERTY()
	class UImage* m_pMarkerIcon;

	UPROPERTY()
	class UImage* m_pArrowIcon;

	UPROPERTY(VisibleAnywhere, Category = "Configuration")
	int m_MaxDisplayedDistance = 999;

	UPROPERTY(EditAnywhere, Category = "Configuration|MarkerType")
	EMarkerTypes m_Type;

public :
	void InitializeWidget(const EMarkerTypes& _Type);
	void UpdateDistance(int _Distance);
	void SetArrowDirection(int _UpDirection);

	/* Get */
	FORCEINLINE class UTextBlock* const& GetDistanceText() const { return m_pDistanceText; }
	FORCEINLINE class UImage*     const& GetMarkerIcon()   const { return m_pMarkerIcon; }
	FORCEINLINE class UImage*     const& GetArrowIcon()    const { return m_pArrowIcon; }

protected :
	virtual void NativePreConstruct() override;

};


