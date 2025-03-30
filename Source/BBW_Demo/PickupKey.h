#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "FlagGamemode.h"
#include "PickupKey.generated.h"

UCLASS()
class BBW_DEMO_API APickupKey : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	APickupKey();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FString GetDescription();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FVector GetUIPosition();

	void Interact_Implementation() override;
	FString GetDescription_Implementation() override;
	FVector GetUIPosition_Implementation() override;

protected:
	virtual void BeginPlay() override;

private:
	void CheckFlagsValidity();

private:
	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	FString mCategory;
	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	FString mFlagName;
	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mMesh;
};
