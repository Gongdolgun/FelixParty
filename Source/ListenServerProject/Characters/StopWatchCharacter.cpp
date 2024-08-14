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

	// ���� ��带 �����ͼ� Ÿ�̸� ����
	AStopWatchGameMode* GameMode = Cast<AStopWatchGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->StopTimer();

		// �������� Ÿ�̸� ������Ʈ ����
		GameMode->CheckResult(GameMode->ElapsedTime); // ���� �ð����� ��� Ȯ��
	}
}

void AStopWatchCharacter::StartTimer()
{

}

void AStopWatchCharacter::StopTimer()
{

}

void AStopWatchCharacter::ServerStopTimer_Implementation()
{

}

void AStopWatchCharacter::MultiStopTimer_Implementation(float Time)
{

}
