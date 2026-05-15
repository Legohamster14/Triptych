// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TriptychGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayers
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerPoints;
};

UCLASS()
class TRIPTYCH2_API UTriptychGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzles")
	int32 TotalPuzzles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzles")
	int32 CompletedPuzzles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScoreBoard")
	TArray<FPlayers> PlayersArray;

};
