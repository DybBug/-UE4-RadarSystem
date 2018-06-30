// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_EnemyMarker.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>

#define LOCTEXT_NAMESPACE "Fromat"

void UWidget_EnemyMarker::UpdateDistance(int _Distance)
{
	if (_Distance > m_MaxDisplayedDistance)
	{
		FText Format = FText::Format(LOCTEXT("Format", "{0}+"), m_MaxDisplayedDistance);
		m_pDistanceText->SetText(Format);
	}
	else
	{
		FText Format = FText::Format(LOCTEXT("Format", "{0}m"), _Distance);
		m_pDistanceText->SetText(Format);
	}
}

#undef LOCTEXT_NAMESPACE
