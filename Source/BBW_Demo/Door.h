#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include <Components/TimelineComponent.h>
#include "FlagTrigger.h"
#include "Door.generated.h"

UCLASS()
class BBW_DEMO_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FString GetDescription();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FVector GetUIPosition();

	void Interact_Implementation() override;
	FString GetDescription_Implementation() override;
	FVector GetUIPosition_Implementation() override;

private:
	UFUNCTION()
	void MoveDoor(float time);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* mCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mTargetPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	USceneComponent* mUiPosition;

	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	UFlagTrigger* mFlagTrigger;

	UTimelineComponent* mTimeline;
	FOnTimelineFloat mCallback{};
	FVector mInitialPosition;

	bool mIsOpen;
};
