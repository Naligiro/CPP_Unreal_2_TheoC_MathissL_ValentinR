#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlagList.h"
#include "FlagManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFlagValueChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BBW_DEMO_API UFlagManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFlagManager();

	UFUNCTION(BlueprintCallable)
	bool IsFlagValid(const FString& category, const FString& flagName) const;
	UFUNCTION(BlueprintCallable)
	void EditFlag(const FString& category, const FString& flagName, const int& value);
	UFUNCTION(BlueprintCallable)
	void IncrementFlag(const FString& category, const FString& flagName);
	UFUNCTION(BlueprintCallable)
	void DecrementFlag(const FString& category, const FString& flagName);
	UFUNCTION(BlueprintCallable)
	void ResetFlagCategory(const FString& category);
	UFUNCTION(BlueprintCallable)
	void ResetAllFlags();
	UFUNCTION(BlueprintCallable)
	bool IsFlagTrigger(const FString& category, const FString& flagName, const int& value, bool exact);

protected:
	virtual void BeginPlay() override;

private:
	void CreateFlags();

	const FString GetFlagKey(const FString& category, const FString& flagName) const;

public:
	FFlagValueChanged OnFlagChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	TArray<FFlagList> Flags;

	TMap<FString, int> mFlagValues;
};
