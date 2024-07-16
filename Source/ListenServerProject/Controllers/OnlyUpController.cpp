#include "Controllers/OnlyUpController.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "Components/TextBlock.h"
#include "Widgets/CharacterOverlay.h"
#include "Widgets/OnlyUpHUD.h"

AOnlyUpController::AOnlyUpController()
{

}

void AOnlyUpController::BeginPlay()
{
	Super::BeginPlay();

	OnlyUpHUD = Cast<AOnlyUpHUD>(GetHUD());
}

void AOnlyUpController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHUDTime();
}

void AOnlyUpController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AOnlyUpCharacter* OnlyUpCharacter = Cast<AOnlyUpCharacter>(InPawn);
	if (OnlyUpCharacter)
	{
		
	}
}

void AOnlyUpController::SetHUDCountdown(float InCountdownTime)
{
	// HUD가 제대로 들어오지 않았을 때 다시 세팅
	if (OnlyUpHUD == nullptr)
	{
		OnlyUpHUD = Cast<AOnlyUpHUD>(GetHUD());
	}

	bool bCheck = OnlyUpHUD &&
		OnlyUpHUD->CharacterOverlay &&
		OnlyUpHUD->CharacterOverlay->CountdownText;

	if (bCheck)
	{
		int32 Minutes = FMath::FloorToInt(InCountdownTime / 60.0f);
		int32 Seconds = InCountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		OnlyUpHUD->CharacterOverlay->CountdownText->SetText(FText::FromString(CountdownText));
	}
}

void AOnlyUpController::SetHUDTime()
{
	float SubTime = MatchTime - GetWorld()->GetTimeSeconds();
	uint32 SecondsLeft = FMath::CeilToInt(SubTime);

	if (CountdownInt != SecondsLeft)
	{
		SetHUDCountdown(SubTime);
	}

	CountdownInt = SecondsLeft;
}
