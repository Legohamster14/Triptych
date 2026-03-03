// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Puzzle/PuzzleGridBase.h"
#include "Components/SphereComponent.h"
#include "Triptych2/Puzzle/PuzzlePin.h"

// Sets default values
APuzzleGridBase::APuzzleGridBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	BigSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Big Sphere"));
	MediumSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Medium Sphere"));
	SmallSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Small Sphere"));

	RootSceneComponent->SetupAttachment(RootComponent);
	Plane->SetupAttachment(RootComponent);
	BigSphereCollider->SetupAttachment(RootSceneComponent);
	MediumSphereCollider->SetupAttachment(BigSphereCollider);
	SmallSphereCollider->SetupAttachment(BigSphereCollider);
}

// Called when the game starts or when spawned
void APuzzleGridBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APuzzleGridBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("Pins is in the %s"), (bPinInCorrectPosition ? TEXT("Right spot") : TEXT("Wrong spot")));

	BigSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APuzzleGridBase::OnBigBeginOverlap);
	BigSphereCollider->OnComponentEndOverlap.AddDynamic(this, &APuzzleGridBase::OnBigEndOverlap);
	MediumSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APuzzleGridBase::OnMediumBeginOverlap);
	MediumSphereCollider->OnComponentEndOverlap.AddDynamic(this, &APuzzleGridBase::OnMediumEndOverlap);
	SmallSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &APuzzleGridBase::OnSmallBeginOverlap);
	SmallSphereCollider->OnComponentEndOverlap.AddDynamic(this, &APuzzleGridBase::OnSmallEndOverlap);
}

void APuzzleGridBase::OnBigBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		bPinInBigRange = true;
	}
}



void APuzzleGridBase::OnBigEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		bPinInBigRange = false;
	}
}

void APuzzleGridBase::OnMediumBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		bPinInMediumRange = true;
	}
}

void APuzzleGridBase::OnMediumEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		bPinInMediumRange = false;
	}
}

void APuzzleGridBase::OnSmallBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		bPinInSmallRange = true;
	}
}

void APuzzleGridBase::OnSmallEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		bPinInSmallRange = false;
	}
}

void APuzzleGridBase::PinCheck()
{
	UE_LOG(LogTemp, Log, TEXT("The Pin is in the: %s, %s, %s"),
		(bPinInBigRange ? TEXT("Big Range") : TEXT("Not Big Range")),
		(bPinInMediumRange ? TEXT("Medium Range") : TEXT("Not Medium Range")),
		(bPinInSmallRange ? TEXT("Small Range") : TEXT("Not Small Range")));
}
