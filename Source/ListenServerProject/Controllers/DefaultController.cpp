#include "Controllers/DefaultController.h"
#include "GameInstances/OnlineGameInstance.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	if (DefaultGameState != nullptr)
		DefaultGameState->OnGameStateTypeChanged.AddDynamic(this, &ADefaultController::WidgetTypeChange_NMC);

	// 매치 시간 초기값 세팅
	GameStartTime = DefaultGameState->GameStartTime;
	GamePlayTime = DefaultGameState->GamePlayTime;
	GameOverTime = DefaultGameState->GameOverTime;

	// Input Mode
	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

	// Option
	if (SelectOptionWidget)
	{
		OptionWidget = CreateWidget<UUserWidget>(GetWorld(), SelectOptionWidget);

	}
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
		TimeLeft = DefaultGameState->GamePlayTime;

		// Percent 계산
		if (TimeLeft > 0.15) 
		{
			Percent = TimeLeft / GamePlayTime; 
			Percent = FMath::Clamp(Percent, 0.0f, 1.0f);
		}
		else
		{
			Percent = 0.0f;
		}

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

	UCharacterOverlay* CharacterOverlay = Cast<UCharacterOverlay>(DefaultHUD->CharacterOverlay);
	if (CharacterOverlay != nullptr)
	{
		CharacterOverlay->CountdownText->SetText(FText::FromString(CountdownText));

		// Percent
		CharacterOverlay->Percent = Percent;
	}
}

void ADefaultController::SetGameStateType()
{
	if (DefaultHUD == nullptr) return;

	UCharacterOverlay* CharacterOverlay = Cast<UCharacterOverlay>(DefaultHUD->CharacterOverlay);
	if (CharacterOverlay != nullptr)
	{
		CharacterOverlay->GameStateType->SetText(FText::FromString(EnumToString(DefaultGameState->GetGameStateType())));
	}
}

void ADefaultController::WidgetTypeChange_NMC_Implementation(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if (DefaultHUD == nullptr) return;

	SetShowMouseCursor(false);

	if (InNewGameType == EGameStateType::GamePlay)
		SetInputMode(FInputModeGameOnly());

	else
	{
		/*ADefaultCharacter* ControlledPawn = Cast<ADefaultCharacter>(GetPawn());
		if(ControlledPawn != nullptr)
		{
			ControlledPawn->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		}*/
		DisableInput(this);
		SetInputMode(FInputModeUIOnly());
	}

	DefaultHUD->ChangeWidgetClass(InPrevGameType, InNewGameType);
}

void ADefaultController::UpdateReadyStatus_Implementation()
{
	if (DefaultGameState == nullptr) return;

	for(auto& PlayerData : DefaultGameState->PlayerDatas)
	{
		if(PlayerData.PlayerName == GetPlayerState<APlayerState>()->GetPlayerName())
		{
			PlayerData.Ready = !PlayerData.Ready;
			break;
		}
	}
}

void ADefaultController::LeaveSessionInProgress_Implementation()
{
	UGameplayStatics::OpenLevel(this, "MainMenu");
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

void ADefaultController::ViewOption()
{
	if (OptionWidget)
	{
		OptionWidget->AddToViewport();

		if (bShowMouseCursor == false)
		{
			SetShowMouseCursor(true);
		}
		
		OptionWidget->SetFocus();
		SetInputMode(FInputModeUIOnly());

	}

}
