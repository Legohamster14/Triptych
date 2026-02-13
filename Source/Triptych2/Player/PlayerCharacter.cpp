// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("CameraYaw", this, &APlayerCharacter::CameraYaw);
	PlayerInputComponent->BindAxis("CamerPitch", this, &APlayerCharacter::CamerPitch);

}

void APlayerCharacter::MoveForward(float InputValue)
{
	FVector fwdVector = GetActorForwardVector();
	AddMovementInput(fwdVector, InputValue);
}

void APlayerCharacter::MoveRight(float InputValue)
{
	FVector rightVector = GetActorRightVector();
	AddMovementInput(rightVector, InputValue);
}

void APlayerCharacter::CameraYaw(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void APlayerCharacter::CamerPitch(float InputValue)
{
	AddControllerPitchInput(InputValue);
}
