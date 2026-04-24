// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Triptych2/Puzzle/PuzzlePin.h"
#include "Triptych2/Puzzle/PuzzleGridBase.h"
#include "Triptych2/Alien/AlienCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	LeftMouseQueryParams.AddIgnoredActor(this);
	EQueryParams.AddIgnoredActor(this);
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Set the position of the carried object in front of the player
	if (bPickedUpObject) {
		FVector GrabbedObjectPos = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * ObjectDistanceFromPlayer;
		GrabbedObjectReference->SetActorLocation(GrabbedObjectPos);
		GrabbedObjectReference->SetActorRotation(PlayerCamera->GetComponentQuat());

	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::EndSprint);

	PlayerInputComponent->BindAxis("CameraYaw", this, &APlayerCharacter::CameraYaw);
	PlayerInputComponent->BindAxis("CamerPitch", this, &APlayerCharacter::CamerPitch);
	PlayerInputComponent->BindAxis("Zoom", this, &APlayerCharacter::CameraZoom);
	PlayerInputComponent->BindAction("ResetZoom", IE_Pressed, this, &APlayerCharacter::ResetZoom);
	PlayerInputComponent->BindAction("QZoom", IE_Pressed, this, &APlayerCharacter::QZoomIn);
	PlayerInputComponent->BindAction("QZoom", IE_Released, this, &APlayerCharacter::QZoomOut);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &APlayerCharacter::LeftMouseInteract);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APlayerCharacter::DropObject);
	PlayerInputComponent->BindAction("E", IE_Pressed, this, &APlayerCharacter::EInteract);

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

void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::CameraYaw(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void APlayerCharacter::CamerPitch(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void APlayerCharacter::CameraZoom(float InputValue)
{
	PlayerCamera->FieldOfView -= InputValue * 2;
	if (PlayerCamera->FieldOfView > 120)
	{
		PlayerCamera->FieldOfView = 120;
	}
	else if (PlayerCamera->FieldOfView < 20) 
	{
		PlayerCamera->FieldOfView = 20;
	}
}

void APlayerCharacter::ResetZoom()
{
	PlayerCamera->FieldOfView = DefaultZoom;
}

void APlayerCharacter::QZoomIn()
{
	PlayerCamera->FieldOfView = QZoomInAmount;
}

void APlayerCharacter::QZoomOut()
{
	PlayerCamera->FieldOfView = DefaultZoom;
}

void APlayerCharacter::LeftMouseInteract()
{
	//Sets up variables for line trace
	FHitResult LeftMouseHitResult;
	FVector LineTraceStart = PlayerCamera->GetComponentLocation();
	FVector LineTraceEnd = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * PlayerReach;

	
	if (!bPickedUpObject) {
		GetWorld()->LineTraceSingleByChannel(LeftMouseHitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Camera, LeftMouseQueryParams);

		//Checks if the player is interacting with a grabbable object
		if (Cast<AGrabbableObject>(LeftMouseHitResult.GetActor()) && Cast<AGrabbableObject>(LeftMouseHitResult.GetActor())->bGrabbable)
		{
			bPickedUpObject = true;
			GrabbedObjectReference = Cast<AGrabbableObject>(LeftMouseHitResult.GetActor());
			Cast<APuzzlePin>(GrabbedObjectReference)->PinMesh->SetMaterial(0, TransarentPinMat);

			if (Cast<APuzzlePin>(GrabbedObjectReference)) {
				LeftMouseQueryParams.AddIgnoredComponent(Cast<APuzzlePin>(GrabbedObjectReference)->PinMesh);
				return;
			}

		}

		//Checks if the player is interacting with an alien
		if (Cast<AAlienCharacter>(LeftMouseHitResult.GetActor())) {
			Cast<AAlienCharacter>(LeftMouseHitResult.GetActor())->PlayerInteract(this);
			return;
		}
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(LeftMouseHitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Camera, LeftMouseQueryParams);

		//Checks if the player is interacting with a puzzle board
		if (Cast<APuzzleGridBase>(LeftMouseHitResult.GetActor()))
		{
			bPickedUpObject = false;
			Cast<APuzzlePin>(GrabbedObjectReference)->PinMesh->SetMaterial(0, OpaquePinMat);
			GrabbedObjectReference->SetActorLocation(LeftMouseHitResult.ImpactPoint);
			GrabbedObjectReference->SetActorRotation((LeftMouseHitResult.ImpactNormal * -180).Rotation());

			Cast<APuzzleGridBase>(LeftMouseHitResult.GetActor())->PinCheck(GrabbedObjectReference);
			LeftMouseQueryParams.SetNumIgnoredComponents(0);
			
			GrabbedObjectReference = nullptr;
		}
	}
}

//Stop the player from carrying an object
void APlayerCharacter::DropObject()
{
	if (bPickedUpObject)
	{
		Cast<APuzzlePin>(GrabbedObjectReference)->PinMesh->SetMaterial(0, OpaquePinMat);
		LeftMouseQueryParams.SetNumIgnoredComponents(0);
		GrabbedObjectReference = nullptr;
		bPickedUpObject = false;
	}
}

void APlayerCharacter::EInteract()
{

}
