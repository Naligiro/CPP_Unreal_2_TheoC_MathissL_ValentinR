#include "InspectionPawn.h"

// Sets default values
AInspectionPawn::AInspectionPawn()
{
 	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Création du pivot de l'objet à inspecter
	mPivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
	mPivot->SetupAttachment(RootComponent);

	//Création de l'objet à inspecter
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(mPivot);

	//Création de la caméra
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(RootComponent);
	mCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	mCamera->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));

	//Création du point auquel l'objet à inspecter est amené quand on possède notre pawn
	//On désactive les collisions et le rend invisible en jeu
	mInspectionPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inspection point"));
	mInspectionPoint->SetupAttachment(mCamera);
	mInspectionPoint->SetRelativeLocation(FVector::ForwardVector);
	mInspectionPoint->SetWorldScale3D(FVector(0.1f));
	mInspectionPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mInspectionPoint->SetHiddenInGame(true);

	//Récupération du mesh de cube pour le visuel par défaut de l'objet à inspecter
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		cubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (cubeVisualAsset.Succeeded())
	{
		mMesh->SetStaticMesh(cubeVisualAsset.Object);
		mInspectionPoint->SetStaticMesh(cubeVisualAsset.Object);
	}

	//Valeurs par défauts
	InspectMappingContext = nullptr;
	RotateAction = nullptr;
	ExitAction = nullptr;
	mPreviousPawn = nullptr;
	mInitialPosition = FVector::ZeroVector;
	mInitialRotation = FRotator::ZeroRotator;
	mControlRotation = FRotator::ZeroRotator;

	mRotationSpeed = 50.0f;
}

// Called when the game starts or when spawned
void AInspectionPawn::BeginPlay()
{
	Super::BeginPlay();

	mInitialPosition = mPivot->GetComponentLocation();
	mInitialRotation = mPivot->GetComponentRotation();
}

// Called every frame
void AInspectionPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateObject(DeltaTime);
}

void AInspectionPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	SetMappingContext(true);
}

// Called to bind functionality to input
void AInspectionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind une fonction à chaque action
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Exit
		EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Started, this, &AInspectionPawn::Exit);
		
		// Rotate
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AInspectionPawn::GetRotationInput);
		// Stop rotation
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &AInspectionPawn::StopRotationInput);
	}
}

void AInspectionPawn::Interact_Implementation()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	APlayerController* controller = world->GetFirstPlayerController();
	if (controller == nullptr)
	{
		return;
	}

	//Récupère le ThirdpersonCharacter
	mPreviousPawn = controller->GetPawn();

	//Positionne la caméra d'inspection sur la caméra du ThirdPersonCharacter
	UCameraComponent* otherCamera = mPreviousPawn->GetComponentByClass<UCameraComponent>();
	mCamera->SetWorldLocation(otherCamera->GetComponentLocation());
	mCamera->SetWorldRotation(otherCamera->GetComponentRotation());

	//Garde en mémoire la rotation du ThirdPersonCharacter
	mControlRotation = controller->GetControlRotation();

	//Possède notre pawn
	controller->UnPossess();
	controller->Possess(this);

	//Positionne l'objet à inspecter devant la caméra
	mPivot->SetWorldLocation(mInspectionPoint->GetComponentLocation());
}

FString AInspectionPawn::GetDescription_Implementation()
{
	return TEXT("Inspect");
}

FVector AInspectionPawn::GetUIPosition_Implementation()
{
	return mPivot->GetComponentLocation();
}

void AInspectionPawn::SetMappingContext(bool value)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (value)
			{
				Subsystem->AddMappingContext(InspectMappingContext, 0);
			}
			else
			{
				Subsystem->RemoveMappingContext(InspectMappingContext);
			}
		}
	}
}

void AInspectionPawn::Exit(const FInputActionValue& Value)
{
	//Replace l'objet à sa position/rotation initiale
	mPivot->SetWorldLocation(mInitialPosition);
	mPivot->SetWorldRotation(mInitialRotation);

	//Désactive les inputs de notre pawn
	SetMappingContext(false);

	//Possède le ThirdPersonCharacter
	AController* controller = GetController();
	controller->UnPossess();
	controller->Possess(mPreviousPawn);
	mPreviousPawn = nullptr;
	mRotationInput = FVector::ZeroVector;
	mInertiaInput = FVector::ZeroVector;

	//Redonnne sa rotation au ThirdPersonCharacter
	controller->SetControlRotation(mControlRotation);
}

void AInspectionPawn::GetRotationInput(const FInputActionValue& Value)
{
	mRotationInput = Value.Get<FVector>();
}

void AInspectionPawn::StopRotationInput(const FInputActionValue& Value)
{
	mRotationInput = FVector::ZeroVector;
}

void AInspectionPawn::RotateObject(const float& deltaTime)
{
	mInertiaInput = FMath::VInterpConstantTo(mInertiaInput, mRotationInput, deltaTime,mRotationSpeed);
	if (mInertiaInput != FVector::ZeroVector)
	{
		//On récupère la rotation de la caméra
		//On ne garde que l'axe vertical
		FRotator cameraRotation = mCamera->GetComponentRotation();
		cameraRotation.Pitch = 0.0f;
		cameraRotation.Roll = 0.0f;

		//On tourne notre input grâce à la rotation de la caméra
		FVector relativeInput = cameraRotation.RotateVector(FVector(-mInertiaInput.Y, 0.0f, 0.0f));

		//On construit notre rotation et on l'ajoute
		//Axe horizontaux : rotation relative à la caméra (axe vertical de la souris)
		//Axe vertical : axe horizontal de la souris
		FRotator rotationToAdd = FRotator(relativeInput.X, -mInertiaInput.X, -relativeInput.Y);
		mPivot->AddWorldRotation(rotationToAdd);
	}
}
