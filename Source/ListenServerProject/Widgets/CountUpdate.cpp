#include "Widgets/CountUpdate.h"

void UCountUpdate::UpdateCount(int32 CurrentCount, int32 MaxCount)
{
	if (CurrentCount && MaxCount)
	{
		Current_Count->SetText(FText::AsNumber(CurrentCount));
		Max_Count->SetText(FText::AsNumber(MaxCount));
	}
}
