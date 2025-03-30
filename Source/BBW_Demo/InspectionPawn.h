// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include "Camera/CameraComponent.h"
#include "Interactable.h"
#include "InspectionPawn.generated.h"

UCLASS()
class BBW_DEMO_API AInspectionPawn : public APawn, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInspectionPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void SetMappingContext(bool value);
	void Exit(const FInputActionValue& Value);
	void GetRotationInput(const FInputActionValue& Value);
	void StopRotationInput(const FInputActionValue& Value);

	void RotateObject(const float& deltaTime);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InspectMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ExitAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects", meta = (AllowPrivateAccess = "true"))
	USceneComponent* mPivot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mInspectionPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
	float mRotationSpeed;

	FVector mRotationInput;
	FVector mInertiaInput;

	APawn* mPreviousPawn;

	FVector mInitialPosition;
	FRotator mInitialRotation;

	FRotator mControlRotation;
};
