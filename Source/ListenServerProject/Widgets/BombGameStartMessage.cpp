#include "Widgets/BombGameStartMessage.h"

void UBombGameStartMessage::SetMessage(const FString& NewMessage)
{
	if (MessageTextBlock)
	{
		MessageTextBlock->SetText(FText::FromString(NewMessage));
	}
}
