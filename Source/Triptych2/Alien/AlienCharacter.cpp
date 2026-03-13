// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Alien/AlienCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Triptych2/Player/PlayerCharacter.h"
#include "Triptych2/Puzzle/PuzzleGridBase.h"
#include "Triptych2/Puzzle/PuzzlePin.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAlienCharacter::AAlienCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Interact Area"));
	AlienText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Alien Text"));
	PinSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Pin Spawn"));
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	InteractArea->SetupAttachment(RootComponent);
	AlienText->SetupAttachment(RootComponent);
	PinSpawn->SetupAttachment(RootComponent);
	AudioComp->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AAlienCharacter::BeginPlay()
{
	Super::BeginPlay();

	AlienChatIndex = 0;
	AlienText->SetText(FText::FromString(AlienChatStrings[AlienChatIndex]));
	
	InteractArea->OnComponentBeginOverlap.AddDynamic(this, &AAlienCharacter::OnInterctAreaBeginOverlap);
	InteractArea->OnComponentEndOverlap.AddDynamic(this, &AAlienCharacter::OnInteractAreaEndOverlap);
	
}


// Called every frame
void AAlienCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAlienCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAlienCharacter::PlayerInteract(APlayerCharacter* Player)
{
	if (!bPuzzleCompleted) {
		if (!bPinSpawned) {
			AlienChatIndex = 1;
			AlienText->SetText(FText::FromString(AlienChatStrings[AlienChatIndex]));
			APuzzlePin* SpawnedPin = this->GetWorld()->SpawnActor<APuzzlePin>(Pin);
			SpawnedPin->SetActorLocation(PinSpawn->GetComponentLocation());
			BoardRef->PinReference = SpawnedPin;
			bPinSpawned = true;
		}
	}
	else
	{
		AlienChatIndex = 2;
		AlienText->SetText(FText::FromString(AlienChatStrings[AlienChatIndex]));
		Player->Points += PointsToAward;
		UGameplayStatics::PlaySound2D(this, CheerSound);
	}
}

void AAlienCharacter::OnInterctAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AAlienCharacter::OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
