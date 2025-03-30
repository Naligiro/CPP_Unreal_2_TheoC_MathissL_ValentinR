#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include <Components/TimelineComponent.h>
#include "FlagTrigger.h"
#include "Chessboard.generated.h"

UCLASS()
class BBW_DEMO_API AChessboard : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChessboard();

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
	void OpenChessboard(float time);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mRightMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mLeftMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* mCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = "true"))
	FVector mLeftTargetRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = "true"))
	FVector mRightTargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	USceneComponent* mUiPosition;

	UPROPERTY(EditAnywhere, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	UFlagTrigger* mFlagTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mRightPivot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mLeftPivot;

	UTimelineComponent* mTimeline;
	FOnTimelineFloat mCallback{};
	FVector mInitialRotation;

	bool mIsOpen;
};