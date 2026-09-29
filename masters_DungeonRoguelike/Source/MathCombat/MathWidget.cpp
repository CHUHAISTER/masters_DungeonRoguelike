// Fill out your copyright notice in the Description page of Project Settings.


#include "MathWidget.h"
#include "MathManager.h"


void UMathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_0) Button_0->OnClicked.AddDynamic(this, &UMathWidget::OnButton0Clicked);
	if (Button_1) Button_1->OnClicked.AddDynamic(this, &UMathWidget::OnButton1Clicked);
	if (Button_2) Button_2->OnClicked.AddDynamic(this, &UMathWidget::OnButton2Clicked);
	if (Button_3) Button_3->OnClicked.AddDynamic(this, &UMathWidget::OnButton3Clicked);
	if (Button_4) Button_4->OnClicked.AddDynamic(this, &UMathWidget::OnButton4Clicked);
	if (Button_5) Button_5->OnClicked.AddDynamic(this, &UMathWidget::OnButton5Clicked);
	if (Button_6) Button_6->OnClicked.AddDynamic(this, &UMathWidget::OnButton6Clicked);
	if (Button_7) Button_7->OnClicked.AddDynamic(this, &UMathWidget::OnButton7Clicked);
	if (Button_8) Button_8->OnClicked.AddDynamic(this, &UMathWidget::OnButton8Clicked);
	if (Button_9) Button_9->OnClicked.AddDynamic(this, &UMathWidget::OnButton9Clicked);

	if (Button_Enter) Button_Enter->OnClicked.AddDynamic(this, &UMathWidget::Click_Enter);
	if (Button_Delete) Button_Delete->OnClicked.AddDynamic(this, &UMathWidget::OnDeletePressed);
	if (Button_minus) Button_minus->OnClicked.AddDynamic(this, &UMathWidget::OnMinusPressed);

	Result->OnTextChanged.AddDynamic(this, &UMathWidget::OnResultTextChanged);

}


void UMathWidget::OnResultTextChanged(const FText& Text)
{
	ResultString = Text.ToString();
}

void UMathWidget::SetTask(UMathManager* MManager)
{
	Manager = MManager;
	Manager->NumberOfTask++;

	if (TaskText)
	{
		TaskText->SetText(Manager->GenerateTask());
	}
}

void UMathWidget::Click_Enter()
{

	if (ResultString.Len() > 0)
	{
		FString ExString = TaskText->GetText().ToString();
		if (FCString::Atoi(*ResultString) != Manager->LastCorrectAnswer)
		{
			ExString += ResultString + "  (-) Correct answer = " + 
				FString::FromInt(Manager->LastCorrectAnswer) + "\n";
			ResultString = FString::Printf(TEXT("Correct answer = %d"), Manager->LastCorrectAnswer);
			UpdateResultText();
			Manager->WrongAnswer++;
			Manager->IsLastCorrectAnswer = false;
			Manager->SkillLevel--;
			if (APlayerController* PC = GetOwningPlayer())
			{
				PC->SetPause(false);
			}
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				this,
				&UMathWidget::RemoveTask,
				1.0f,
				false
			);
			Manager->Examples.Add(ExString);
			Manager->AddStreakElement(0);
			OnAnswerFinished.Broadcast(false);
			return;
		}
		ExString += ResultString + "\n";
		Manager->Examples.Add(ExString);
		Manager->AddStreakElement(1);
		Manager->SkillLevel++;

		RemoveTask();
		Manager->CorrectAnswer++;
		Manager->IsLastCorrectAnswer = true;
		OnAnswerFinished.Broadcast(true);
	}
}

void UMathWidget::RemoveTask()
{
	RemoveFromParent();
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
		PC->SetPause(false);
	}
}

void UMathWidget::OnNumberPressed(FString Number)
{
	ResultString += Number;
	UpdateResultText();
}

void UMathWidget::OnDeletePressed()
{
	if (ResultString.Len() > 0)
	{
		ResultString.LeftChopInline(1);
		UpdateResultText();
	}
}

void UMathWidget::UpdateResultText()
{
	if (Result)
	{
		Result->SetText(FText::FromString(ResultString));
	}
}
