#include "Widgets/StartTimer.h"

void UStartTimer::UpdateTimer(float CurrentTime)
{
	TimerText->SetText(FText::AsNumber(CurrentTime));
	//TimerText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), CurrentTime)));
}

void UStartTimer::PauseTimer()
{

}
