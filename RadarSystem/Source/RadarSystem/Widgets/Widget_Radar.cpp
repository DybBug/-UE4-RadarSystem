// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Radar.h"
#include "Widget_Direction.h"
#include "Widget_Marker.h"
#include "Widget_Landmark.h"
#include "Widget_EnemyMarker.h"
#include "RadarSystemEnums.h"

#include <Animation/WidgetAnimation.h>
#include <Components/Overlay.h>
#include <Components/OverlaySlot.h>
#include <Components/Image.h>

void UWidget_Radar::AddDirection(const FDirectionInfo& _DirectionInfo)
{
	UClass* pWB_DirectionClass = LoadClass<UWidget_Direction>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_Direction.WB_Direction_C'"));
	if(pWB_DirectionClass)
	{
		// #. 방향 위젯 생성 및 초기화.
		UWidget_Direction* pWidget = CreateWidget<UWidget_Direction>(GetWorld(), pWB_DirectionClass);
		pWidget->InitializeWidget(_DirectionInfo);

		// #. 방향 위젯 추가.
		m_DirectionWidgets.Emplace(pWidget);

		// #. 레이더의 오버레이에 방향 위젯 추가.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. 오버레이에 추가된 위젯들(방향 위젯) 수평/수직 정렬.
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // 수평 중앙 정렬.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);     // 수직 하단 정렬. 
	}
}

void UWidget_Radar::AddMarker(const EMarkerTypes& _Type)
{
	UClass* pWB_MarkerClass = LoadClass<UWidget_Marker>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_Marker.WB_Marker_C'"));
	if(pWB_MarkerClass)
	{
		// #. 표식 위젯 생성 및 초기화.
		UWidget_Marker* pWidget = CreateWidget<UWidget_Marker>(GetWorld(), pWB_MarkerClass);
		pWidget->InitializeWidget(_Type);

		// #. 표식 위젯 추가.
		m_MarkerWidgets.Emplace(pWidget);

		// #. 레이더의 오버레이에 표식 위젯 추가.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. 오버레이에 추가된 위젯들(표식 위젯) 수평/수직 정렬 및 패딩 설정..
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // 수평 중앙 정렬.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);     // 수직 중앙 정렬. 
		pOverlaySlot->SetPadding(FMargin(0.f, 5.f, 0.f, 4.f));                     // 패딩 설정(표식 위젯이 레이더의 정 중앙에 오게 패딩을 설정 한다.)
	}
}


void UWidget_Radar::AddLandmark(UTexture2D* const& _pIcon)
{
	UClass* pWB_LandmarkClass = LoadClass<UWidget_Landmark>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_Landmark.WB_Landmark_C'"));
	if(pWB_LandmarkClass)
	{
		// #. 랜드마크 위젯 생성 및 초기화.
		UWidget_Landmark* pWidget = CreateWidget<UWidget_Landmark>(GetWorld(), pWB_LandmarkClass);
		pWidget->InitializeWidget(_pIcon);

		// #. 랜드마크  위젯 추가.
		m_LandmarkWidgets.Emplace(pWidget);

		// #. 레이더의 오버레이에 랜드마크  위젯 추가.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. 오버레이에 추가된 위젯들(랜드마크 위젯) 수평/수직 정렬.
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // 수평 중앙 정렬.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);     // 수직 중앙 정렬. 
	}
}

void UWidget_Radar::AddEnemyMarker()
{
	UClass* pWB_EnemyMarkerClass = LoadClass<UWidget_EnemyMarker>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_EnemyMarker.WB_EnemyMarker_C'"));
	if(pWB_EnemyMarkerClass)
	{
		// #. 적표식 위젯 생성 및 초기화.
		UWidget_EnemyMarker* pWidget = CreateWidget<UWidget_EnemyMarker>(GetWorld(), pWB_EnemyMarkerClass);	

		// #. 적표식  위젯 추가.
		m_EnemyMarkerWidgets.Emplace(pWidget);

		// #. 레이더의 오버레이에 적표식  위젯 추가.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. 오버레이에 추가된 위젯들(적표식 위젯) 수평/수직 정렬 및 패딩 설정..
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // 수평 중앙 정렬.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);     // 수직 중앙 정렬. 
		pOverlaySlot->SetPadding(FMargin(0.f, 0.f, 0.f, 7.f));                     // 패딩 설정(표식 위젯이 레이더의 정 중앙에 오게 패딩을 설정 한다.)
	}
}

void UWidget_Radar::RemoveMarker(int _Index)
{
	m_MarkerWidgets[_Index]->RemoveFromParent();
	m_MarkerWidgets.RemoveAt(_Index);
}

void UWidget_Radar::RemoveEnemyMarker(int _Index)
{
	m_EnemyMarkerWidgets[_Index]->RemoveFromParent();
	m_EnemyMarkerWidgets.RemoveAt(_Index);
}

void UWidget_Radar::HighlightRadar(bool _bHighlight)
{
	FLinearColor Color = _bHighlight ? FColor(0xFFFFE200) : FColor(0xB3FFFFFF);
	m_pRadar->SetColorAndOpacity(Color);
}

bool UWidget_Radar::Initialize()
{
	bool bResult = Super::Initialize();

	return bResult ? _SetupAnimation() : false;
}

bool UWidget_Radar::_SetupAnimation()
{
	UProperty* pProperty = GetClass()->PropertyLink;

	// #.이 클래스의 모든 속성을 실행하여 위젯 애니메이션을 찾기.
	while (pProperty != nullptr)
	{
		// #.찾은 속성이 '오브젝트 속성'이라면...
		if (pProperty->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* pObjProperty = Cast<UObjectProperty>(pProperty);

			// #. '오브젝트 속성'이 위젯 애니메이션이라면...
			if (pObjProperty->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* pObj = pObjProperty->GetObjectPropertyValue_InContainer(this, 0);

				UWidgetAnimation* pWidgetAnim = Cast<UWidgetAnimation>(pObj);

				if (pWidgetAnim != nullptr && pWidgetAnim->MovieScene != nullptr)
				{
					m_pBlendOut = pWidgetAnim;
					return true;
				}
			}
		}
		// #. 다음 속성으로 이동.
		pProperty = pProperty->PropertyLinkNext;
	}

	return false;
}


