#include "Controllers/DefaultController.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "GameState/DefaultGameState.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/CharacterOverlay.h"
#include "Widgets/DefaultHUD.h"

ADefaultController::ADefaultController()
{
}

void ADefaultController::BeginPlay()
{
	Super::BeginPlay();


	DefaultHUD = Cast<ADefaultHUD>(GetHUD());
	DefaultGameState = Cast<ADefaultGameState>(UGameplayStatics::GetGameState(this));
}

void ADefaultController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetHUDTime();
	SetGameStateType();
}

void ADefaultController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ADefaultCharacter* DefaultCharacter = Cast<ADefaultCharacter>(InPawn);
	if (DefaultCharacter)
	{

	}
}

void ADefaultController::SetHUDTime()
{
	if (DefaultHUD == nullptr) return;
	if (DefaultGameState == nullptr) return;

	switch (DefaultGameState->GetGameStateType())
	{
	case EGameStateType::GameStart:
		TimeLeft = DefaultGameState->GameStartTime;
		break;

	case EGameStateType::GamePlay:
		TimeLeft = DefaultGameState->GameMatchTime;
		break;

	case EGameStateType::GameOver:
		TimeLeft = DefaultGameState->GameOverTime;
		break;

	default:
		break;
	}

	uint32 CountdownInt = FMath::CeilToInt(TimeLeft);

	// CountText
	int32 Minutes = FMath::FloorToInt(CountdownInt / 60.f);
	int32 Seconds = CountdownInt - Minutes * 60;

	FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	DefaultHUD->CharacterOverlay->CountdownText->SetText(FText::FromString(CountdownText));
}

void ADefaultController::SetGameStateType()
{
	if (DefaultHUD == nullptr) return;

	DefaultHUD->CharacterOverlay->GameStateType->SetText(FText::FromString(EnumToString(DefaultGameState->GetGameStateType())));
}

FString ADefaultController::EnumToString(EGameStateType InGameStateType)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGameStateType"), true);
	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	return EnumPtr->GetNameByValue((int64)InGameStateType).ToString();
}

