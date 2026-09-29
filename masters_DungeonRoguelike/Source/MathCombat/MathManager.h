// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MathWidget.h"


#include "MathManager.generated.h"

class ADRPlayerController;


/**
 * 
 */
UCLASS()
class MATHCOMBAT_API UMathManager : public UObject
{
	GENERATED_BODY()
public:
	int32 CorrectAnswer = 0;
	int32 WrongAnswer = 0;
	int32 NumberOfTask = 0;
	TArray<FString> Examples;
	int32 LastCorrectAnswer = 0;
	bool IsLastCorrectAnswer;
	int32 SkillLevel = 50;
	TArray<int32> MathStreak = {-1, -1, -1};
	const int32 MaxStreakElement = 3;
	FText GenerateTask();


	void AddStreakElement(int32 NewElement);

	bool AreAllStreakElementsEqual();
};
