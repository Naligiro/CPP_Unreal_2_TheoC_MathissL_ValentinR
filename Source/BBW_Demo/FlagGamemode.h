#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameHUD.h"
#include "FlagManager.h"
#include "FlagGamemode.generated.h"

UCLASS()
class BBW_DEMO_API AFlagGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFlagGamemode();

	UFUNCTION(BlueprintCallable, Category = "Flag")
	UFlagManager* GetFlagManager() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	UFlagManager* mFlagManager;
};
