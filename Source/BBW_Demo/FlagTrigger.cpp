#include "FlagTrigger.h"

UFlagTrigger::UFlagTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UFlagTrigger::IsFlagTriggered()
{
	//On récupère le flag manager
	//Si on n'arrive pas à le récupérer on renvoit faux pour éviter
	//des actions qui sne devrait pas s'executer
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return false;
	}

	AFlagGamemode* gamemode = Cast<AFlagGamemode>(world->GetAuthGameMode());
	if (gamemode == nullptr)
	{
		return false;
	}

	//On passe par tout les flags
	UFlagManager* flagManager = gamemode->GetFlagManager();
	for (int i = 0; i < mFlags.Num(); i++)
	{
		FString category = mFlags[i].Category;
		FString flagName = mFlags[i].FlagName;
		int value = mFlags[i].Value;
		bool exact = mFlags[i].Exact;

		//Si au moins une valeur d'un flag n'est pas bonne on renvoit faux
		if (!flagManager->IsFlagTrigger(category, flagName, value, exact))
		{
			return false;
		}
	}

	//Sinon on renvoit vrai
	return true;
}

void UFlagTrigger::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		FTimerHandle timerHandle;
		world->GetTimerManager().SetTimer(timerHandle, this, &UFlagTrigger::CheckFlagsValidity, 0.001f, false);
	}
}

void UFlagTrigger::BeginDestroy()
{
	Super::BeginDestroy();

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check : can't get world"));
		return;
	}

	AFlagGamemode* gamemode = Cast<AFlagGamemode>(world->GetAuthGameMode());
	if (gamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check : can't get gamemode"));
		return;
	}

	UFlagManager* flagManager = gamemode->GetFlagManager();

	if (mListen)
	{
		flagManager->OnFlagChanged.RemoveDynamic(this, &UFlagTrigger::FlagTriggerCallback);
	}
}

void UFlagTrigger::CheckFlagsValidity()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check : can't get world"));
	}

	AFlagGamemode* gamemode = Cast<AFlagGamemode>(world->GetAuthGameMode());
	if (gamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check : can't get gamemode"));
	}

	UFlagManager* flagManager = gamemode->GetFlagManager();

	for (int i = 0; i < mFlags.Num(); i++)
	{
		if (flagManager->IsFlagValid(mFlags[i].Category, mFlags[i].FlagName))
		{
			UE_LOG(LogTemp, Error, TEXT("Flag doesn't exist : $s : $s"), *mFlags[i].Category, *mFlags[i].FlagName);
		}
	}

	if (mListen)
	{
		flagManager->OnFlagChanged.AddDynamic(this, &UFlagTrigger::FlagTriggerCallback);
	}
}

void UFlagTrigger::FlagTriggerCallback()
{
	if (IsFlagTriggered())
	{
		OnFlagChanged.Broadcast();
	}
}
