// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TriptychGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRIPTYCH2_API UTriptychGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzles")
	int32 TotalPuzzles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzles")
	int32 CompletedPuzzles;

};
