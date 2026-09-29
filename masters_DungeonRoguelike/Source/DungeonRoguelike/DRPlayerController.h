	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HealthBar.h"
#include "MathCombat/MathWidget.h"


#include "DRPlayerController.generated.h"

class UMathManager;
/**
 * 
 */
UCLASS()
class DUNGEONROGUELIKE_API ADRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UHealthBar> UHealthBarClass;
private:
	UHealthBar* HealthBarWidget;

	
public:
	UHealthBar* GetUHealthBar() {return HealthBarWidget;	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMathWidget> MathWidgetClass;

	UPROPERTY()
	UMathManager* MathManager;

	UPROPERTY()
	UMathWidget* MathWidgetInstance;

	void StartTaskMenu();
	
	UFUNCTION()
	void OnMathWidgetAnswer(bool bIsCorrect);
	
	void SaveExamplesToFile();



	bool bCanShoot;

};
