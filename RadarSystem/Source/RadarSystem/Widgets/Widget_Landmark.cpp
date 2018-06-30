// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Landmark.h"

#include <Components/Image.h>
#include <Engine/Texture2D.h>

void UWidget_Landmark::InitializeWidget(UTexture2D* const& _pIcon)
{
	m_pLandmarkIcon = _pIcon;
}

void UWidget_Landmark::NativePreConstruct()
{
	if (m_pLandmarkIcon)
	{
		FSlateBrush Brush;
		Brush.ImageSize = FVector2D(m_pLandmarkIcon->GetSizeX(), m_pLandmarkIcon->GetSizeY());
		Brush.SetResourceObject(m_pLandmarkIcon);
		m_pIcon->SetBrush(Brush);
	}
}
