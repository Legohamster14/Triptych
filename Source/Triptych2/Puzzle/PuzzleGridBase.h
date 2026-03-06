// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleGridBase.generated.h"

class USphereComponent;

UCLASS()
class TRIPTYCH2_API APuzzleGridBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleGridBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Reference")
	class APuzzlePin* PinReference;

	bool bPinInBigRange = false;
	bool bPinInMediumRange = false;
	bool bPinInSmallRange = false;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UTextRenderComponent* BoardText;

	UFUNCTION()
	void PinCheck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBigBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBigEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnMediumBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMediumEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSmallBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSmallEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* Plane;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* BigSphereCollider;
	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* MediumSphereCollider;
	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* SmallSphereCollider;

};
