#include "Characters/OnlyUpCharacter.h"

AOnlyUpCharacter::AOnlyUpCharacter()
{
}

void AOnlyUpCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AOnlyUpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOnlyUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
