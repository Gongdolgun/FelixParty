#include "Characters/StopWatchCharacter.h"
#include "Global.h"
#include "EnhancedInputComponent.h"
#include "GameModes/StopWatchGameMode.h"
#include "Net/UnrealNetwork.h"

AStopWatchCharacter::AStopWatchCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

void AStopWatchCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		if (StartTimerWidgetClass)
		{
			StartTimerWidget = CreateWidget<UStartTimer>(GetWorld(), StartTimerWidgetClass);
			if (StartTimerWidget)
			{
				StartTimerWidget->AddToViewport();
				StartTimerWidget->SetVisibility(ESlateVisibility::Hidden);

				GetWorld()->GetTimerManager().SetTimer(StartTimerWidgetHandle, this, &AStopWatchCharacter::StartWidgetViewport, 3.0f, false);
			}
		}
	}
}

void AStopWatchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStopWatchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &AStopWatchCharacter::Action);

	}
}

void AStopWatchCharacter::Action()
{
	Super::Action();

	if (HasAuthority())
	{
		MultiStopButton();
	}

	else
	{
		ServerStopButton();
	}
}

void AStopWatchCharacter::StopButton()
{
	// 게임 모드를 가져와서 타이머 중지
	AStopWatchGameMode* GameMode = Cast<AStopWatchGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->StopTimer();

		StopTime = GameMode->ElapsedTime;
		GameMode->CheckResult(GameMode->ElapsedTime); // 멈춘 시간으로 결과 확인
	}
}

void AStopWatchCharacter::ServerStopButton_Implementation()
{
	MultiStopButton();
}

void AStopWatchCharacter::MultiStopButton_Implementation()
{
	StopButton();
}

void AStopWatchCharacter::UpdateTimer(float UpdateTime)
{
	if (StartTimerWidget)
	{
		StartTimerWidget->UpdateTimer(UpdateTime);
		CLog::Log(UpdateTime);
	}
}

void AStopWatchCharacter::StartWidgetViewport()
{
	if (StartTimerWidget)
	{
		StartTimerWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// 서버에서 모든 클라이언트로 호출
	if (HasAuthority())
	{
		MultiStartWidgetViewport();
	}
}

void AStopWatchCharacter::MultiStartWidgetViewport_Implementation()
{
	if (StartTimerWidget)
	{
		StartTimerWidget->SetVisibility(ESlateVisibility::Visible);
	}
}


