#include "Door.h"

ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Cr�ation du root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Cr�ation du mesh de la porte
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(RootComponent);

	//Cr�ation du point d'arriv�e de la porte
	mTargetPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Position"));
	mTargetPosition->SetupAttachment(RootComponent);
	mTargetPosition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Uniquement visible dans l'�diteur
	mTargetPosition->bHiddenInGame = true;
	mTargetPosition->SetWorldScale3D(FVector(0.1f));

	//R�cup�ration du mesh de cube pour donner une valeur par d�faut
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		cubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (cubeVisualAsset.Succeeded())
	{
		mMesh->SetStaticMesh(cubeVisualAsset.Object);
		mTargetPosition->SetStaticMesh(cubeVisualAsset.Object);
	}

	mUiPosition = CreateDefaultSubobject<USceneComponent>(TEXT("UI Position"));
	mUiPosition->SetupAttachment(RootComponent);

	//Cr�ation de la timeline
	mTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	//Stock les fonctions qui vont �tre appel�es par la timeline
	mCallback.BindUFunction(this, FName{ TEXT("MoveDoor") });

	mFlagTrigger = CreateDefaultSubobject<UFlagTrigger>(TEXT("Flag trigger"));

	mInitialPosition = FVector::ZeroVector;
	mIsOpen = false;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	//Donne la curve et les callback � la timeline
	mTimeline->AddInterpFloat(mCurve, mCallback);

	//Stock la position de la porte avant ouverture
	mInitialPosition = mMesh->GetComponentLocation();

	mFlagTrigger->OnFlagChanged.AddDynamic(this, &ADoor::Interact);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::Interact_Implementation()
{
	if (!mIsOpen && mFlagTrigger->IsFlagTriggered())
	{
		mTimeline->Play();
		mIsOpen = true;
	}
}

FString ADoor::GetDescription_Implementation()
{
	if (mIsOpen)
	{
		return TEXT("");
	}
	return TEXT("Open");
}

FVector ADoor::GetUIPosition_Implementation()
{
	return mUiPosition->GetComponentLocation();
}

void ADoor::MoveDoor(float time)
{
	FVector position = FMath::Lerp(mInitialPosition, mTargetPosition->GetComponentLocation(), time);
	mMesh->SetWorldLocation(position);
}