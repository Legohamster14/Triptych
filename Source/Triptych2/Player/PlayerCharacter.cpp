// Fill out your copyright notice in the Description page of Project Settings.


#include "Triptych2/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Triptych2/Puzzle/PuzzlePin.h"
#include "Triptych2/Puzzle/PuzzleGridBase.h"

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

	if (bPickedUpObject) {
		FVector GrabbedObjectPos = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * ObjectDistanceFromPlayer;
		GrabbedObjectReference->SetActorLocation(GrabbedObjectPos);
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("CameraYaw", this, &APlayerCharacter::CameraYaw);
	PlayerInputComponent->BindAxis("CamerPitch", this, &APlayerCharacter::CamerPitch);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &APlayerCharacter::LeftMouseInteract);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APlayerCharacter::DropObject);

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

void APlayerCharacter::LeftMouseInteract()
{
	//UE_LOG(LogTemp, Log, TEXT("has player picked up an object %s"), (bPickedUpObject ? TEXT("True") : TEXT("False")));
	FHitResult LeftMouseHitResult;
	FVector LineTraceStart = PlayerCamera->GetComponentLocation();
	FVector LineTraceEnd = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * PlayerReach;
	LeftMouseQueryParams.AddIgnoredActor(this);

	if (!bPickedUpObject) {
		GetWorld()->LineTraceSingleByChannel(LeftMouseHitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Camera, LeftMouseQueryParams);
		//DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, LeftMouseHitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);
		//UE_LOG(LogTemp, Log, TEXT("Line Trace Has Hit: %s"), *LeftMouseHitResult.GetActor()->GetName());

		if (Cast<AGrabbableObject>(LeftMouseHitResult.GetActor()))
		{
			bPickedUpObject = true;
			GrabbedObjectReference = Cast<AGrabbableObject>(LeftMouseHitResult.GetActor());

			if (Cast<APuzzlePin>(GrabbedObjectReference)) {
				LeftMouseQueryParams.AddIgnoredComponent(Cast<APuzzlePin>(GrabbedObjectReference)->PinMesh);
				Cast<APuzzlePin>(GrabbedObjectReference)->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

		}
	}
	else
	{
		GetWorld()->LineTraceSingleByChannel(LeftMouseHitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Camera, LeftMouseQueryParams);
		//DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, LeftMouseHitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);
		//UE_LOG(LogTemp, Log, TEXT("Line Trace Has Hit: %s"), *LeftMouseHitResult.GetActor()->GetName());

		if (Cast<APuzzleGridBase>(LeftMouseHitResult.GetActor()))
		{
			bPickedUpObject = false;
			if (Cast<APuzzlePin>(GrabbedObjectReference)) {
				Cast<APuzzlePin>(GrabbedObjectReference)->Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}

			GrabbedObjectReference->SetActorLocation(LeftMouseHitResult.ImpactPoint);
			GrabbedObjectReference->SetActorRotation(LeftMouseHitResult.ImpactNormal.Rotation() * 180);

			LeftMouseQueryParams.SetNumIgnoredComponents(0);
			
			Cast<APuzzleGridBase>(LeftMouseHitResult.GetActor())->PinCheck();
			GrabbedObjectReference = nullptr;
		}
	}
}

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
