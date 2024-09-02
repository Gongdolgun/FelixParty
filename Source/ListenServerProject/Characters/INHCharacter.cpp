#include "Characters/INHCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "GameState/DefaultGameState.h"
#include "Net/UnrealNetwork.h"

AINHCharacter::AINHCharacter()
{
}

void AINHCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AINHCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (canRun)
		CurrentSpeed = FMath::Lerp(CurrentSpeed, 500.f, 2 * DeltaSeconds);

	else
		CurrentSpeed = FMath::Lerp(CurrentSpeed, 230.f, 2 * DeltaSeconds);

	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

void AINHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ThisClass::SetSpeed, true);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ThisClass::SetSpeed, false);
	}
}

void AINHCharacter::Action()
{
	Super::Action();
}

void AINHCharacter::End_Action()
{
	Super::End_Action();
}

void AINHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, canRun);
	DOREPLIFETIME(ThisClass, CurrentSpeed);
}

void AINHCharacter::SetSpeed_Server_Implementation(bool InCanRun)
{
	canRun = InCanRun;
}

void AINHCharacter::SetSpeed(bool InIsRun)
{
	isRun = InIsRun;

	if (isRun)
		SetSpeed_Server(true);

	else
		SetSpeed_Server(false);
}