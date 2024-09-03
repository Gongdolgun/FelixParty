#include "Widgets/StartTimer.h"

void UStartTimer::UpdateTimer(float CurrentTime)
{
	if (TimerText)
	{
		TimerText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), FMath::Clamp(CurrentTime, 0.0f, 60.0f))));
	}
}

void UStartTimer::UpdateRandomTime(float RandomTime)
{
	if (RandomTimeText)
	{
		RandomTimeText->SetText(FText::FromString(FString::Printf(TEXT("Random Time : %.2f"), RandomTime)));

	}
}

