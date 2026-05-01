// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AlienCharacter.generated.h"

class APuzzlePin;
class UMaterial;

UENUM()
enum class EFacialExpression : uint8
{
	Sad, Neutral, Happy1, Happy2, Happy3, Questioning
};

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

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<UMaterial*> FacialExpressions;

	bool bPuzzleCompleted = false;

	UFUNCTION()
	void PlayerInteract(class APlayerCharacter* Player);

	UFUNCTION()
	void ChangeFacialExpression(EFacialExpression FacialExpression);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UTextRenderComponent* AlienText;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* PinSpawn;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UAudioComponent* AudioComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* FacePlane;

	bool bPinSpawned = false;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bGivenPoints = false;

	UPROPERTY(EditAnywhere, Category = "Game Instance")
	class UTriptychGameInstance* TriptychGI;

};
