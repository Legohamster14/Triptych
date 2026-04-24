// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UMaterialInstance;

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

	UPROPERTY(EditAnywhere, Category = "Interaction")
	int32 PlayerReach = 500;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	int32 ObjectDistanceFromPlayer = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Points = 0;

	UPROPERTY(EditAnywhere, Category = "Movement")
	int32 SprintSpeed = 1000;
	UPROPERTY(EditAnywhere, Category = "Movement")
	int32 WalkSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UMaterialInstance* OpaquePinMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UMaterialInstance* TransarentPinMat;

	UPROPERTY(EditAnywhere, Category = "Camera")
	int32 DefaultZoom = 90;
	UPROPERTY(EditAnywhere, Category = "Camera")
	int32 QZoomInAmount = 40;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* PlayerCamera;

	void MoveForward(float InputValue);
	void MoveRight(float InputValue);
	void StartSprint();
	void EndSprint();

	void CameraYaw(float InputValue);
	void CamerPitch(float InputValue);
	void CameraZoom(float InputValue);
	void ResetZoom();
	void QZoomIn();
	void QZoomOut();

	void LeftMouseInteract();
	void DropObject();
	void EInteract();

	bool bPickedUpObject = false;

	FCollisionQueryParams LeftMouseQueryParams;
	FCollisionQueryParams EQueryParams;

	class AGrabbableObject* GrabbedObjectReference;

	class APuzzleGridBase* PuzzleBoardRef;

};
