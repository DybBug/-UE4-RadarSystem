// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Marker.h"
#include "RadarSystemEnums.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>

#define LOCTEXT_NAMESPACE "Format"

void UWidget_Marker::InitializeWidget(const EMarkerTypes& _Type)
{
	m_Type = _Type;
}

void UWidget_Marker::UpdateDistance(int _Distance)
{
	if (_Distance > m_MaxDisplayedDistance)
	{
		FText Format = FText::Format(LOCTEXT("Format", "{0}+"), m_MaxDisplayedDistance);
		m_pDistanceText->SetText(Format);
	}
	else
	{
		_Distance = FMath::Clamp<int>(_Distance, 0, m_MaxDisplayedDistance);
		FText Format = FText::Format(LOCTEXT("Format", "{0}m"), _Distance);
		m_pDistanceText->SetText(Format);
	}
}

void UWidget_Marker::SetArrowDirection(int _UpDirection)
{
	if (_UpDirection != 0)
	{
		m_pArrowIcon->SetRenderScale(FVector2D(1.f, _UpDirection));
		m_pArrowIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_pArrowIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidget_Marker::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (m_Type == EMarkerTypes::MainQuest)
	{
		UTexture2D* pIcon = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_MainQuest.Icon_MainQuest'"));
		m_pMarkerIcon->SetBrushFromTexture(pIcon, true);
		m_pArrowIcon->SetColorAndOpacity(FColor(0xFFF58E2A));
	}
	else if (m_Type == EMarkerTypes::SideQuest)
	{
		UTexture2D* pIcon = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_SideQuest.Icon_SideQuest'"));
		m_pMarkerIcon->SetBrushFromTexture(pIcon, true);
		m_pArrowIcon->SetColorAndOpacity(FColor(0xFF7AC1FF));
	}	
}

#undef LOCTEXT_NAMESPACE

