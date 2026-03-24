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
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void APlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::CameraYaw(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void APlayerCharacter::CamerPitch(float InputValue)
{
	AddControllerPitchInput(InputValue);
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

			if (Cast<APuzzlePin>(GrabbedObjectReference)) {
				LeftMouseQueryParams.AddIgnoredComponent(Cast<APuzzlePin>(GrabbedObjectReference)->PinMesh);
				Cast<APuzzlePin>(GrabbedObjectReference)->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
			if (Cast<APuzzlePin>(GrabbedObjectReference)) {
				Cast<APuzzlePin>(GrabbedObjectReference)->Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}

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
		if (Cast<APuzzlePin>(GrabbedObjectReference)) {
			Cast<APuzzlePin>(GrabbedObjectReference)->Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		LeftMouseQueryParams.SetNumIgnoredComponents(0);
		GrabbedObjectReference = nullptr;
		bPickedUpObject = false;
	}
}

void APlayerCharacter::EInteract()
{

}
