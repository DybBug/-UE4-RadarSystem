// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadarSystemStructure.h"
#include "Widget_Radar.generated.h"


enum class EMarkerTypes : uint8;
/**
 * 
 */
UCLASS()
class RADARSYSTEM_API UWidget_Radar : public UUserWidget
{
	GENERATED_BODY()	

protected :
	UPROPERTY()
	class UOverlay* m_pOverlay;
	UPROPERTY()
	class UImage* m_pRadar;
	UPROPERTY()
	class UWidgetAnimation* m_pBlendOut;

	UPROPERTY(VisibleAnywhere, Category = "Configuration|Widgtes")
	TArray<class UWidget_Direction*> m_DirectionWidgets;

	UPROPERTY(VisibleAnywhere, Category = "Configuration|Widgtes")
	TArray<class UWidget_Marker*> m_MarkerWidgets;

	UPROPERTY(VisibleAnywhere, Category = "Configuration|Widgtes")
	TArray<class UWidget_Landmark*> m_LandmarkWidgets;

	UPROPERTY(VisibleAnywhere, Category = "Configuration|Widgtes")
	TArray<class UWidget_EnemyMarker*> m_EnemyMarkerWidgets;

public :
	// #. ���� ������ ���� ���� ���� �߰�.
	void AddDirection(const FDirectionInfo& _DirectionInfo);

	// #. ǥ�� Ÿ�Կ� ���� ǥ�� ���� �߰�.
	void AddMarker(const EMarkerTypes& _Type);

	// #. ���帶ũ ���� �߰�.
	void AddLandmark(UTexture2D* const& _pIcon);

	// #. ��ǥ�� ���� �߰�.
	void AddEnemyMarker();

	// #. ǥ�� Ÿ�� ����.
	void RemoveMarker(int _Index);

	// #. ��ǥ�� ����.
	void RemoveEnemyMarker(int _Index);

	void HighlightRadar(bool _bHighlight);

	/* Get */
	FORCEINLINE class UOverlay*         const& GetOverlay()  const { return m_pOverlay; }
	FORCEINLINE class UImage*           const& GetRadar()    const { return m_pRadar; }
	FORCEINLINE class UWidgetAnimation* const& GetBlendOut() const { return m_pBlendOut; }

	FORCEINLINE TArray<class UWidget_Direction*>   const& GetDirectionWidgets()   const { return m_DirectionWidgets; }
	FORCEINLINE TArray<class UWidget_Marker*>      const& GetMarkerWidgets()      const { return m_MarkerWidgets; }
	FORCEINLINE TArray<class UWidget_Landmark*>    const& GetLandmarkWidgets()    const { return m_LandmarkWidgets; }
	FORCEINLINE TArray<class UWidget_EnemyMarker*> const& GetEnemyMarkerWidgets() const { return m_EnemyMarkerWidgets; }

	/* Set */

protected :
	virtual bool Initialize() override;

protected :
	bool _SetupAnimation();
	
	
};
