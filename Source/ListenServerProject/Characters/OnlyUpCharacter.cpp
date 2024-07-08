#include "Characters/OnlyUpCharacter.h"
#include "Components/ParkourComponent.h"
#include "Global.h"

AOnlyUpCharacter::AOnlyUpCharacter()
{
	Helpers::CreateActorComponent<UParkourComponent>(this, &ParkourComponent, "ParkourComponent");
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

void AOnlyUpCharacter::Action()
{
	Super::Action();

	// Action에서 파쿠르를? 스페이스바로 사용하게 하는게 좋을듯
}
