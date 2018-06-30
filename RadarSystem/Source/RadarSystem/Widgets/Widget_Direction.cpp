// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Direction.h"

#include <Components/TextBlock.h>


void UWidget_Direction::InitializeWidget(const FDirectionInfo & _DirectionInfo)
{
	m_DirectionInfo = _DirectionInfo;
}

void UWidget_Direction::NativePreConstruct()
{
	Super::NativePreConstruct();

	FString NameStr = m_DirectionInfo.Name.ToString();

	NameStr = NameStr.LeftChop(NameStr.Len()-2);
	NameStr = NameStr.ToUpper();
	m_pText->SetText(FText::FromString(NameStr));
}


