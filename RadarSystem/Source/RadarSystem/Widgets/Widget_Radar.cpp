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
		// #. ���� ���� ���� �� �ʱ�ȭ.
		UWidget_Direction* pWidget = CreateWidget<UWidget_Direction>(GetWorld(), pWB_DirectionClass);
		pWidget->InitializeWidget(_DirectionInfo);

		// #. ���� ���� �߰�.
		m_DirectionWidgets.Emplace(pWidget);

		// #. ���̴��� �������̿� ���� ���� �߰�.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. �������̿� �߰��� ������(���� ����) ����/���� ����.
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // ���� �߾� ����.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);     // ���� �ϴ� ����. 
	}
}

void UWidget_Radar::AddMarker(const EMarkerTypes& _Type)
{
	UClass* pWB_MarkerClass = LoadClass<UWidget_Marker>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_Marker.WB_Marker_C'"));
	if(pWB_MarkerClass)
	{
		// #. ǥ�� ���� ���� �� �ʱ�ȭ.
		UWidget_Marker* pWidget = CreateWidget<UWidget_Marker>(GetWorld(), pWB_MarkerClass);
		pWidget->InitializeWidget(_Type);

		// #. ǥ�� ���� �߰�.
		m_MarkerWidgets.Emplace(pWidget);

		// #. ���̴��� �������̿� ǥ�� ���� �߰�.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. �������̿� �߰��� ������(ǥ�� ����) ����/���� ���� �� �е� ����..
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // ���� �߾� ����.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);     // ���� �߾� ����. 
		pOverlaySlot->SetPadding(FMargin(0.f, 5.f, 0.f, 4.f));                     // �е� ����(ǥ�� ������ ���̴��� �� �߾ӿ� ���� �е��� ���� �Ѵ�.)
	}
}


void UWidget_Radar::AddLandmark(UTexture2D* const& _pIcon)
{
	UClass* pWB_LandmarkClass = LoadClass<UWidget_Landmark>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_Landmark.WB_Landmark_C'"));
	if(pWB_LandmarkClass)
	{
		// #. ���帶ũ ���� ���� �� �ʱ�ȭ.
		UWidget_Landmark* pWidget = CreateWidget<UWidget_Landmark>(GetWorld(), pWB_LandmarkClass);
		pWidget->InitializeWidget(_pIcon);

		// #. ���帶ũ  ���� �߰�.
		m_LandmarkWidgets.Emplace(pWidget);

		// #. ���̴��� �������̿� ���帶ũ  ���� �߰�.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. �������̿� �߰��� ������(���帶ũ ����) ����/���� ����.
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // ���� �߾� ����.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);     // ���� �߾� ����. 
	}
}

void UWidget_Radar::AddEnemyMarker()
{
	UClass* pWB_EnemyMarkerClass = LoadClass<UWidget_EnemyMarker>(nullptr, TEXT("WidgetBlueprint'/Game/RadarSystem/Widgets/WB_EnemyMarker.WB_EnemyMarker_C'"));
	if(pWB_EnemyMarkerClass)
	{
		// #. ��ǥ�� ���� ���� �� �ʱ�ȭ.
		UWidget_EnemyMarker* pWidget = CreateWidget<UWidget_EnemyMarker>(GetWorld(), pWB_EnemyMarkerClass);	

		// #. ��ǥ��  ���� �߰�.
		m_EnemyMarkerWidgets.Emplace(pWidget);

		// #. ���̴��� �������̿� ��ǥ��  ���� �߰�.
		UOverlaySlot* pOverlaySlot = m_pOverlay->AddChildToOverlay(pWidget);

		// #. �������̿� �߰��� ������(��ǥ�� ����) ����/���� ���� �� �е� ����..
		pOverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center); // ���� �߾� ����.
		pOverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);     // ���� �߾� ����. 
		pOverlaySlot->SetPadding(FMargin(0.f, 0.f, 0.f, 7.f));                     // �е� ����(ǥ�� ������ ���̴��� �� �߾ӿ� ���� �е��� ���� �Ѵ�.)
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

	// #.�� Ŭ������ ��� �Ӽ��� �����Ͽ� ���� �ִϸ��̼��� ã��.
	while (pProperty != nullptr)
	{
		// #.ã�� �Ӽ��� '������Ʈ �Ӽ�'�̶��...
		if (pProperty->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* pObjProperty = Cast<UObjectProperty>(pProperty);

			// #. '������Ʈ �Ӽ�'�� ���� �ִϸ��̼��̶��...
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
		// #. ���� �Ӽ����� �̵�.
		pProperty = pProperty->PropertyLinkNext;
	}

	return false;
}


