// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include <Components/EditableTextBox.h>


#include "MathWidget.generated.h"

class UMathManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnswerFinished, bool, bIsCorrect);

/**
 * 
 */
UCLASS()
class MATHCOMBAT_API UMathWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TaskText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_Enter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_Delete;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr <UEditableTextBox> Result;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_minus;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_5;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_6;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_7;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_8;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_9;

	UFUNCTION()
	void OnButton0Clicked() { OnNumberPressed("0"); }
	UFUNCTION()
	void OnButton1Clicked() { OnNumberPressed("1"); }
	UFUNCTION()
	void OnButton2Clicked() { OnNumberPressed("2"); }
	UFUNCTION()
	void OnButton3Clicked() { OnNumberPressed("3"); }
	UFUNCTION()
	void OnButton4Clicked() { OnNumberPressed("4"); }
	UFUNCTION()
	void OnButton5Clicked() { OnNumberPressed("5"); }
	UFUNCTION()
	void OnButton6Clicked() { OnNumberPressed("6"); }
	UFUNCTION()
	void OnButton7Clicked() { OnNumberPressed("7"); }
	UFUNCTION()
	void OnButton8Clicked() { OnNumberPressed("8"); }
	UFUNCTION()
	void OnButton9Clicked() { OnNumberPressed("9"); }

	UFUNCTION()
	void OnMinusPressed() { OnNumberPressed("-"); }

	UFUNCTION()
	void OnNumberPressed(FString Number);

	UFUNCTION()
	void OnDeletePressed();

	void UpdateResultText();

	FString ResultString;

public:
	UFUNCTION(BlueprintCallable)
	void OnResultTextChanged(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void SetTask(UMathManager* Manager);

	UMathManager* Manager;

	UFUNCTION()
	void Click_Enter();

	void RemoveTask();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAnswerFinished OnAnswerFinished;
};
