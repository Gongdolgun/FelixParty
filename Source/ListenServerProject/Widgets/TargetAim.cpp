#include "Widgets/TargetAim.h"
#include "Global.h"
#include "Components/Image.h"

void UTargetAim::UpdateTargetAimPosition(const FVector& Position)
{
	if (TargetAimImage)
	{
		FVector2D ScreenPostion;

		if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), Position, ScreenPostion))
		{
			TargetAimImage->SetRenderTranslation(ScreenPostion);
		}
	}
}
