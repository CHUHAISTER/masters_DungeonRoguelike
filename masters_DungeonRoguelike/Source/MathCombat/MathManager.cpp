// Fill out your copyright notice in the Description page of Project Settings.


#include "MathManager.h"

FText UMathManager::GenerateTask()
{
	int32 Limit;
	if (SkillLevel > 100) { SkillLevel = 100; }
	int32 SkillChalange = SkillLevel / 10;
	if (SkillChalange <= 5) 
	{
		Limit = 10;
	}
	else if (SkillChalange == 6) 
	{
		Limit = 25;
	}
	else if (SkillChalange == 7)
	{
		Limit = 40;
	}
	else if (SkillChalange == 8)
	{
		Limit = 55;
	}
	else if (SkillChalange == 9)
	{
		Limit = 70;
	}
	else if (SkillChalange == 10)
	{
		Limit = 100;
	}
	
	int32 First = FMath::RandRange(-Limit, Limit);
	int32 Second = FMath::RandRange(-Limit, Limit);
	int32 TaskType = FMath::RandRange(0, 3); 
	FString TaskString;

	switch (TaskType)
	{
	case 0:
		TaskString = FString::Printf(TEXT("%d - %d = "), First, Second);
		LastCorrectAnswer = First - Second;
		break;
	case 1:
		TaskString = FString::Printf(TEXT("%d * %d = "), First, Second);
		LastCorrectAnswer = First * Second;
		break;
	case 2:
		if (First % Second == 0) {
			TaskString = FString::Printf(TEXT("%d / %d = "), First, Second);
			LastCorrectAnswer = First / Second;
		}
	case 3:
		TaskString = FString::Printf(TEXT("%d + %d = "), First, Second);
		LastCorrectAnswer = First + Second;
		break;
	}
    

    UE_LOG(LogTemp, Warning, TEXT("result(manager) = %d"), LastCorrectAnswer);

    return FText::FromString(TaskString);
}

void UMathManager::AddStreakElement(int32 NewElement)
{
	if (MathStreak.Num() >= MaxStreakElement)
	{
		MathStreak.RemoveAt(0);
	}
	MathStreak.Add(NewElement);
}

bool UMathManager::AreAllStreakElementsEqual()
{
	if (MathStreak.Num() == 0)
		return false;

	const int32 First = MathStreak[0];
	for (const int32 Elem : MathStreak)
	{
		if (!(Elem == First))
			return false;
	}
	return true;
}



