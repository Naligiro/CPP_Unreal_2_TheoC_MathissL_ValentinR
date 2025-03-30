// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

void AGameHUD::SetInteractionDescription(const FString& description)
{
	mInteractionWidget->SetDescription(description);
}

void AGameHUD::SetInteractionPosition(const FVector2D& position)
{
	mInteractionWidget->SetPosition(position);
}

void AGameHUD::BeginPlay()
{
	mInteractionWidget = Cast<UInteractionWidget>(CreateWidget(GetWorld(), mInteractionWidgetPrefab));
	mInteractionWidget->AddToViewport();
}
