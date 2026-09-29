// Fill out your copyright notice in the Description page of Project Settings.
#include "DRPlayerController.h"
#include "MathCombat/MathManager.h"
#include "Blueprint/UserWidget.h"
#include "DungeonRoguelikeCharacter.h"

void ADRPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController() && UHealthBarClass)
    {
        HealthBarWidget = CreateWidget<UHealthBar>(this, UHealthBarClass);
        if (HealthBarWidget)
        {
            HealthBarWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("HealthBarWidget successfully generated!"));
        }
    }
    MathManager = NewObject<UMathManager>();
}

void ADRPlayerController::StartTaskMenu()
{
    bCanShoot = false;
    MathWidgetInstance = CreateWidget<UMathWidget>(this, MathWidgetClass);
    MathWidgetInstance->OnAnswerFinished.AddDynamic(this, &ADRPlayerController::OnMathWidgetAnswer);
    MathWidgetInstance->SetTask(MathManager);
    MathWidgetInstance->AddToViewport();


}
void  ADRPlayerController::OnMathWidgetAnswer(bool bIsCorrect)
{
    ADungeonRoguelikeCharacter* MyChar = Cast<ADungeonRoguelikeCharacter>(GetPawn());

    if (bIsCorrect)
    {
        MyChar->CountProjectile = 2;
        MyChar->SpawnProjectile();
    }
    else 
    {
        MyChar->TakeDamage(1);
    }
    if (MathManager->AreAllStreakElementsEqual())
    {
        if (MathManager->IsLastCorrectAnswer)
        {
            MyChar->TakeDamage(-5);
            MyChar->CountProjectile++;
            MathManager->SkillLevel += 4;

        }
        else
        {
            MyChar->TakeDamage(3);
            MathManager->SkillLevel -= 4;

        }
        MathManager->MathStreak = {-1, -1, -1};
    }
    
}
void ADRPlayerController::SaveExamplesToFile()
{
    FString DateTimeStr = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
    FString Header = FString::Printf(TEXT("Data and time saving: %s"), *DateTimeStr);

    TArray<FString> Lines;
    Lines.Add(Header);
    Lines.Append(MathManager->Examples);
    Lines.Add("Number of correct answers = " + FString::FromInt(MathManager->CorrectAnswer) + "\n");
    Lines.Add("Number of wrong answers = " + FString::FromInt(MathManager->WrongAnswer) + "\n");
    Lines.Add("Number of task = " + FString::FromInt(MathManager->NumberOfTask) + "\n");
    Lines.Add("Àinal grade = " + FString::FromInt(MathManager->SkillLevel) + "\n");

    FString SaveDir = FPaths::ProjectSavedDir();
    FString FilePath = SaveDir / TEXT("ExamplesLog.txt");

    FFileHelper::SaveStringArrayToFile(Lines, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8);
    UE_LOG(LogTemp, Warning, TEXT("SAVE! FilePath: %s"), *FilePath);
}
