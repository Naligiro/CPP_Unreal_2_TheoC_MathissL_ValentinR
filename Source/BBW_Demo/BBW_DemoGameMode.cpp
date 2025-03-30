// Copyright Epic Games, Inc. All Rights Reserved.

#include "BBW_DemoGameMode.h"
#include "BBW_DemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABBW_DemoGameMode::ABBW_DemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
