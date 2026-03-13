// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienCharacter.generated.h"

class APuzzlePin;

UCLASS()
class TRIPTYCH2_API AAlienCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAlienCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<FString> AlienChatStrings;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	int32 AlienChatIndex = 0;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	int32 PointsToAward = 0;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<APuzzlePin> Pin;
	UPROPERTY(EditAnywhere, Category = "Interaction")
	class APuzzleGridBase* BoardRef;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	class USoundBase* CheerSound;

	bool bPuzzleCompleted = false;

	UFUNCTION()
	void PlayerInteract(class APlayerCharacter* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* InteractArea;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UTextRenderComponent* AlienText;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* PinSpawn;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	class UAudioComponent* AudioComp;
	bool bPinSpawned = false;


	UFUNCTION()
	void OnInterctAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
