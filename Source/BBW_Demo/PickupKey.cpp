#include "PickupKey.h"

APickupKey::APickupKey()
{
	PrimaryActorTick.bCanEverTick = true;

	// Création du root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Création du mesh de la porte
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(RootComponent);
}

void APickupKey::Interact_Implementation()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check: cannot get world"));
		return;
	}

	AFlagGamemode* gamemode = Cast<AFlagGamemode>(world->GetAuthGameMode());
	if (gamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check: cannot get gamemode"));
		return;
	}

	UFlagManager* flagManager = gamemode->GetFlagManager();

	flagManager->IncrementFlag(mCategory, mFlagName);
	Destroy();
}

FString APickupKey::GetDescription_Implementation()
{
	return TEXT("Pick");
}

FVector APickupKey::GetUIPosition_Implementation()
{
	return GetActorLocation();
}

void APickupKey::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		FTimerHandle timerHandle;
		world->GetTimerManager().SetTimer(timerHandle, this, &APickupKey::CheckFlagsValidity, 0.001f, false);
	}
}

void APickupKey::CheckFlagsValidity()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check: cannot get world"));
		return;
	}

	AFlagGamemode* gamemode = Cast<AFlagGamemode>(world->GetAuthGameMode());
	if (gamemode == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Flag check: cannot get gamemode"));
		return;
	}

	UFlagManager* flagManager = gamemode->GetFlagManager();

	if (!flagManager->IsFlagValid(mCategory, mFlagName))
		UE_LOG(LogTemp, Error, TEXT("Flag doesn't exist: %s: %s"), *mCategory, *mFlagName);
}