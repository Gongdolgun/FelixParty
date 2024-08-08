#include "Widgets/BombGameStartMessage.h"

void UBombGameStartMessage::NativeConstruct()
{
	Super::NativeConstruct();

	/*Index = 0;

	Condition = true;

	StartTimer();*/
}

void UBombGameStartMessage::StartTimer()
{
	/*if (Condition)
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
		GetWorld()->GetTimerManager().SetTimer(Handle, this, &UBombGameStartMessage::RemoveWidget, 1.0f, false);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBombGameStartMessage::UpdateText, TypeSpeed, true);
	}*/
}

void UBombGameStartMessage::RemoveWidget()
{
	//this->RemoveFromViewport();
}

void UBombGameStartMessage::UpdateText()
{
	/*if (Index < InputText.Len())
	{
		DisPlayedText.AppendChar(InputText[Index]);
		Index++;

		if (MessageTextBlock)
		{
			MessageTextBlock->SetText(FText::FromString(DisPlayedText));
		}

		if (Index >= InputText.Len())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}*/
}
