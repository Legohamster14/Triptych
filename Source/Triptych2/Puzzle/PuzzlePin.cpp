// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Puzzle/PuzzlePin.h"
#include "Components/SphereComponent.h"

// Sets default values
APuzzlePin::APuzzlePin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pin Mesh"));
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));

	RootSceneComponent->SetupAttachment(RootComponent);
	PinMesh->SetupAttachment(RootSceneComponent);
	Collider->SetupAttachment(RootSceneComponent);

}

// Called when the game starts or when spawned
void APuzzlePin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzlePin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

