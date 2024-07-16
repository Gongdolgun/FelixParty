#include "Controllers/OnlyUpController.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "Components/TextBlock.h"
#include "GameState/OnlyUpGameState.h"
#include "Widgets/CharacterOverlay.h"
#include "Widgets/OnlyUpHUD.h"

AOnlyUpController::AOnlyUpController()
{

}

void AOnlyUpController::BeginPlay()
{
	Super::BeginPlay();

	OnlyUpHUD = Cast<AOnlyUpHUD>(GetHUD());
	OnlyUpGameState = Cast<ADefaultGameState>(UGameplayStatics::GetGameState(this));
}

void AOnlyUpController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHUDTime();
	SetGameStateType();

	//CLog::LogAndPrint(EnumToString(OnlyUpGameState->GetGameStateType()));
}

void AOnlyUpController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AOnlyUpCharacter* OnlyUpCharacter = Cast<AOnlyUpCharacter>(InPawn);
	if (OnlyUpCharacter)
	{

	}
}

void AOnlyUpController::SetHUDTime()
{
	if (OnlyUpHUD == nullptr) return;
	if (OnlyUpGameState == nullptr) return;

	switch (OnlyUpGameState->GetGameStateType())
	{
	case EGameStateType::GameStart:
		TimeLeft = OnlyUpGameState->GameStartTime;
		break;

	case EGameStateType::GamePlay:
		TimeLeft = OnlyUpGameState->GameMatchTime;
		break;

	case EGameStateType::GameOver:
		TimeLeft = OnlyUpGameState->GameOverTime;
		break;
	}

	uint32 CountdownInt = FMath::CeilToInt(TimeLeft);

	// CountText
	int32 Minutes = FMath::FloorToInt(CountdownInt / 60.f);
	int32 Seconds = CountdownInt - Minutes * 60;

	FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	OnlyUpHUD->CharacterOverlay->CountdownText->SetText(FText::FromString(CountdownText));
}

FString AOnlyUpController::EnumToString(EGameStateType InGameStateType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGameStateType"), true);
	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	return EnumPtr->GetNameByValue((int64)InGameStateType).ToString();
}

void AOnlyUpController::SetGameStateType()
{
	if (OnlyUpHUD == nullptr) return;

	OnlyUpHUD->CharacterOverlay->GameStateType->SetText(FText::FromString(EnumToString(OnlyUpGameState->GetGameStateType())));

}
