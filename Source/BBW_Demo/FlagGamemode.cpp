#include "FlagGamemode.h"

AFlagGamemode::AFlagGamemode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AGameHUD> GameHUD(TEXT("/Game/BP_GameHUD"));
	if (GameHUD.Class != NULL)
	{
		HUDClass = GameHUD.Class;
	}

	mFlagManager = CreateDefaultSubobject<UFlagManager>(TEXT("Flag Manager"));
}

UFlagManager* AFlagGamemode::GetFlagManager() const
{
	return mFlagManager;
}
