#include "Chessboard.h"

// Sets default values
AChessboard::AChessboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Création du root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mRightPivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPivot"));
	mRightPivot->SetupAttachment(RootComponent);
	mLeftPivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPivot"));
	mLeftPivot->SetupAttachment(RootComponent);

	//création du mesh de la porte droite
	mRightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightMesh"));
	mRightMesh->SetupAttachment(mRightPivot);

	//création du mesh de la porte gauche
	mLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftMesh"));
	mLeftMesh->SetupAttachment(mLeftPivot);

	//récupérer le mesh du cube pour donner une valeur par defaut
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		cubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (cubeVisualAsset.Succeeded())
	{
		mRightMesh->SetStaticMesh(cubeVisualAsset.Object);
		mLeftMesh->SetStaticMesh(cubeVisualAsset.Object);
	}

	mUiPosition = CreateDefaultSubobject<USceneComponent>(TEXT("UI Position"));
	mUiPosition->SetupAttachment(RootComponent);

	//création de la timeline 
	mTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	mFlagTrigger = CreateDefaultSubobject<UFlagTrigger>(TEXT("Flag trigger"));

	//stock les fonctions qui vont être appelées par la timeline
	mCallback.BindUFunction(this, FName{ TEXT("ActivateChessboard") });
	mInitialRotation = FVector::ZeroVector;
	mIsOpen = false;
}

// Called when the game starts or when spawned
void AChessboard::BeginPlay()
{
	Super::BeginPlay();

	//donne la curve et les callback à la timeline
	mTimeline->AddInterpFloat(mCurve, mCallback);

	mFlagTrigger->OnFlagChanged.AddDynamic(this, &AChessboard::Interact);

}

// Called every frame
void AChessboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessboard::Interact_Implementation()
{
	if (!mIsOpen && mFlagTrigger->IsFlagTriggered())
	{
		mTimeline->Play();
		mIsOpen = true;
	}
}

FString AChessboard::GetDescription_Implementation()
{
	if (mIsOpen)
		return TEXT("");
	return TEXT("Open");
}

FVector AChessboard::GetUIPosition_Implementation()
{
	return mUiPosition->GetComponentLocation();
}

void AChessboard::ActivateChessboard(float time)
{
	FRotator Rightrotation = FRotator(FQuat::Slerp(mInitialRotation.ToOrientationQuat(), mRightTargetRotation.ToOrientationQuat(), time));
	FRotator Leftrotation = FRotator(FQuat::Slerp(mInitialRotation.ToOrientationQuat(), mLeftTargetRotation.ToOrientationQuat(), time));
	mRightPivot->SetWorldRotation(Rightrotation);
	mLeftPivot->SetWorldRotation(Leftrotation);
}