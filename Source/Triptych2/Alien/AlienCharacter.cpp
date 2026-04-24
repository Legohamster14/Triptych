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

	AlienText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Alien Text"));
	PinSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Pin Spawn"));
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	FacePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Face Plane"));

	AlienText->SetupAttachment(RootComponent);
	PinSpawn->SetupAttachment(RootComponent);
	AudioComp->SetupAttachment(RootComponent);
	FacePlane->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void AAlienCharacter::BeginPlay()
{
	Super::BeginPlay();

	AlienChatIndex = 0;
	AlienText->SetText(FText::FromString(AlienChatStrings[AlienChatIndex]));
	FacePlane->SetMaterial(0, FacialExpressions[1]);
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
	//
	if (!bPuzzleCompleted) {
		if (!bPinSpawned) {
			AlienChatIndex = 1;
			AlienText->SetText(FText::FromString(AlienChatStrings[AlienChatIndex]));
			ChangeFacialExpression(EFacialExpression::Questioning);
			APuzzlePin* SpawnedPin = this->GetWorld()->SpawnActor<APuzzlePin>(Pin);
			SpawnedPin->SetActorLocation(PinSpawn->GetComponentLocation());
			BoardRef->PinReference = SpawnedPin;
			bPinSpawned = true;
		}
	}
	else if (bGivenPoints == false)
	{
		AlienChatIndex = 2;
		AlienText->SetText(FText::FromString(AlienChatStrings[AlienChatIndex]));
		Player->Points += PointsToAward;
		UGameplayStatics::PlaySound2D(this, CheerSound);
		bGivenPoints = true;
	}
}

void AAlienCharacter::ChangeFacialExpression(EFacialExpression FacialExpression)
{
	//UE_LOG(LogTemp, Display, TEXT("Change Facial expression"));
	switch (FacialExpression)
	{
	case EFacialExpression::Sad:
		//UE_LOG(LogTemp, Display, TEXT("Change Facial expression to sad"));
		FacePlane->SetMaterial(0, FacialExpressions[0]);
		break;
	case EFacialExpression::Neutral:
		FacePlane->SetMaterial(0, FacialExpressions[1]);
		break;
	case EFacialExpression::Happy1:
		FacePlane->SetMaterial(0, FacialExpressions[2]);
		break;
	case EFacialExpression::Happy2:
		FacePlane->SetMaterial(0, FacialExpressions[3]);
		break;
	case EFacialExpression::Happy3:
		FacePlane->SetMaterial(0, FacialExpressions[4]);
		break;
	case EFacialExpression::Questioning:
		FacePlane->SetMaterial(0, FacialExpressions[5]);
		break;
	default:
		break;
	}
}
