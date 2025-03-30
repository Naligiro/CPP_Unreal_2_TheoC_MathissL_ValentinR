#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Flag.h"
#include "FlagGamemode.h"
#include "FlagTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BBW_DEMO_API UFlagTrigger : public UActorComponent
{
	GENERATED_BODY()

public:
	UFlagTrigger();

	UFUNCTION(BlueprintCallable)
	bool IsFlagTriggered();

	UPROPERTY(BlueprintAssignable)
	FFlagValueChanged OnFlagChanged;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	void CheckFlagsValidity();
	UFUNCTION()
	void FlagTriggerCallback();

private:
	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	TArray<FFlag> mFlags;
	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	bool mListen;
};
