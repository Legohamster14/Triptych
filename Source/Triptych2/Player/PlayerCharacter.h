// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TRIPTYCH2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int32 PlayerReach = 500;

	int32 ObjectDistanceFromPlayer = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Points = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* PlayerCamera;

	void MoveForward(float InputValue);
	void MoveRight(float InputValue);

	void CameraYaw(float InputValue);
	void CamerPitch(float InputValue);

	void LeftMouseInteract();
	void DropObject();
	void EInteract();

	bool bPickedUpObject = false;

	FCollisionQueryParams LeftMouseQueryParams;
	FCollisionQueryParams EQueryParams;

	class AGrabbableObject* GrabbedObjectReference;

	class APuzzleGridBase* PuzzleBoardRef;

};
