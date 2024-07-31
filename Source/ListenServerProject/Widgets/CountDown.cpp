#include "Widgets/CountDown.h"

void UCountDown::UpdateCountdown(int CountdownValue)
{
    if (CountdownText)
    {
        CountdownText->SetText(FText::AsNumber(CountdownValue));
    }
}
