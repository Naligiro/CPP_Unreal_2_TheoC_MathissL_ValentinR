#include "PlayerInteraction.h"

// Sets default values for this component's properties
UPlayerInteraction::UPlayerInteraction()
{
	PrimaryComponentTick.bCanEverTick = true;

	mCamera = nullptr;
	mInteractionRange = 0.0f;
}

// Called when the game starts
void UPlayerInteraction::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPlayerInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractDetection();
	SetInteractionPosition();
}

void UPlayerInteraction::UseInteraction()
{
	if (mInteractable == nullptr)
	{
		return;
	}

	IInteractable::Execute_Interact(mInteractable);
}

void UPlayerInteraction::InteractDetection()
{
	FHitResult hit;

	ECollisionChannel channel(ECC_GameTraceChannel1);
	const FCollisionQueryParams traceParam(FName(TEXT("")), false, GetOwner());

	FVector startPosition = mCamera->GetComponentLocation();
	FVector endPosition = startPosition + mCamera->GetForwardVector() * mInteractionRange;

	if (!GetWorld()->LineTraceSingleByChannel(OUT hit, startPosition, endPosition, channel, traceParam))
	{
		SetInteractionDescription("");
		mInteractable = nullptr;
		return;
	}

	AActor* hitActor = hit.GetActor();
	if (hitActor == nullptr)
	{
		SetInteractionDescription("");
		mInteractable = nullptr;
		return;
	}

	if (hitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		mInteractable = hitActor;
		FString description = IInteractable::Execute_GetDescription(mInteractable);
		SetInteractionDescription(description);
	}
	else
	{
		SetInteractionDescription("");
		mInteractable = nullptr;
	}
}

bool UPlayerInteraction::TryGetController()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return false;
	}

	mController = world->GetFirstPlayerController();
	if (mController == nullptr)
	{
		return false;
	}
	return true;
}

bool UPlayerInteraction::TryGetHUD()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return false;
	}
	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr)
	{
		return false;
	}

	//Cast renvoie nill si le cast échoue
	mHud = Cast<AGameHUD>(controller->GetHUD());
	if (mHud == nullptr)
	{
		return false;
	}

	return true;
}

void UPlayerInteraction::SetInteractionDescription(const FString& description)
{
	//Si le hud n'est pas null on s'en sert, TryGetHUD n'est même pas appelée
	//Sinon on essaye de le récupérer
	//Si TryGetHUD renvoie true, on a réussi à le récupérer donc on s'en sert
	if (mHud != nullptr || TryGetHUD())
	{
		mHud->SetInteractionDescription(description);
	}
}

void UPlayerInteraction::SetInteractionPosition()
{
	//S'il n'y a pas d'objet interactif devant nous, il n'y a rien à placer
	if (mInteractable == nullptr)
	{
		return;
	}

	if (mController != nullptr || TryGetController())
	{
		FVector worldPosition = IInteractable::Execute_GetUIPosition(mInteractable);

		FVector2D uiPosition = FVector2D::ZeroVector;
		//La fonction prend en paramètre une référence à uiPosition, elle va donc directement modifier notre variable
		UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(mController, worldPosition, uiPosition, true);
		if (mHud != nullptr || TryGetHUD())
		{
			mHud->SetInteractionPosition(uiPosition);
		}
	}
}
