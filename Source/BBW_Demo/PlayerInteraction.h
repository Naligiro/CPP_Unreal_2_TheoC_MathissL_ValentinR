#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Interactable.h"
#include "GameHUD.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include "PlayerInteraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BBW_DEMO_API UPlayerInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInteraction();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(BlueprintCallable)
	void UseInteraction();

	void InteractDetection();

	bool TryGetController();
	bool TryGetHUD();
	void SetInteractionDescription(const FString& description);
	void SetInteractionPosition();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* mCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float mInteractionRange;

	AActor* mInteractable;

	AGameHUD* mHud;

	APlayerController* mController;
};
