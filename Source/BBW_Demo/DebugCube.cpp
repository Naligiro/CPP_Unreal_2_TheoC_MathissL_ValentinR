// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugCube.h"

// Sets default values
ADebugCube::ADebugCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		cubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (cubeVisualAsset.Succeeded())
	{
		m_Mesh->SetStaticMesh(cubeVisualAsset.Object);
		m_Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void ADebugCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADebugCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AddActorLocalRotation(FRotator(0.0f, m_RotationSpeed * DeltaTime, 0.0f));

	m_Mesh->AddLocalRotation(FRotator(0.0f, m_RotationSpeed * DeltaTime, 0.0f));
}

void ADebugCube::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));
}

FString ADebugCube::GetDescription_Implementation()
{
	return TEXT("Debug");
}

FVector ADebugCube::GetUIPosition_Implementation()
{
	return GetActorLocation();
}

