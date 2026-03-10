// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Puzzle/PuzzleGridBase.h"
#include "Components/SphereComponent.h"
#include "Triptych2/Puzzle/PuzzlePin.h"
#include "Components/TextRenderComponent.h"
#include "Internationalization/Text.h"
#include "Triptych2/Player/PlayerCharacter.h"
#include "Triptych2/Alien/AlienCharacter.h"

// Sets default values
APuzzleGridBase::APuzzleGridBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	Map = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Map"));
	RandomPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RandomPosition"));
	BigSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Big Sphere"));
	MediumSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Medium Sphere"));
	SmallSphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Small Sphere"));
	BoardText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Board Text"));

	RootSceneComponent->SetupAttachment(RootComponent);
	Map->SetupAttachment(RootSceneComponent);
	RandomPosition->SetupAttachment(RootSceneComponent);
	BigSphereCollider->SetupAttachment(RootSceneComponent);
	MediumSphereCollider->SetupAttachment(BigSphereCollider);
	SmallSphereCollider->SetupAttachment(BigSphereCollider);
	BoardText->SetupAttachment(RootSceneComponent);

	RandomPosition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APuzzleGridBase::BeginPlay()
{
	Super::BeginPlay();
	BoardText->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoardText->SetText(FText::FromString("Can you guess where this is?"));
	PlayerRef = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
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

void APuzzleGridBase::PinCheck(AActor* OtherActor)
{
	if (Cast<APuzzlePin>(OtherActor) == PinReference) {
		if (bPinInBigRange) {
			PinReference->bGrabbable = false;
			AlienRef->bPuzzleCompleted = true;
		}

		if (bPinInSmallRange)
		{
			BoardText->SetText(FText::FromString("Spot On! You Get 3 Points!"));
			AlienRef->PointsToAward = 3;
		}
		else if (bPinInMediumRange)
		{
			BoardText->SetText(FText::FromString("You are very close! You Get 2 Points!"));
			AlienRef->PointsToAward = 2;
		}
		else if (bPinInBigRange) {
			BoardText->SetText(FText::FromString("You are close! You Get 1 Point!"));
			AlienRef->PointsToAward = 1;
		}
		else
		{
			BoardText->SetText(FText::FromString("Keep Trying!"));
		}
	}
}
