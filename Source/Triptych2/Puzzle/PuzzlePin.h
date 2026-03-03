// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Triptych2/Puzzle/GrabbableObject.h"
#include "PuzzlePin.generated.h"

UCLASS()
class TRIPTYCH2_API APuzzlePin : public AGrabbableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzlePin();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* PinMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class USphereComponent* Collider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
