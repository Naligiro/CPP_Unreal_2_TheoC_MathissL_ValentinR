// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

void UInteractionWidget::SetDescription(const FString& description)
{
	mDescriptionBlock->SetText(FText::FromString(description));
}

void UInteractionWidget::SetPosition(const FVector2D& position)
{
	Cast<UCanvasPanelSlot>(mDescriptionBlock->Slot)->SetPosition(position);
}

void UInteractionWidget::NativeConstruct()
{
	mDescriptionBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Description")));
	if (mDescriptionBlock == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Description widget : text block not found"));
	}
}
