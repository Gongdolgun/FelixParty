#include "Actors/LobbyPlatforms.h"
#include "Global.h"
#include "Characters/LobbyCharacter.h"
#include "Components/ArrowComponent.h"
#include "Controllers/LobbyController.h"

ALobbyPlatforms::ALobbyPlatforms()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent(this, &Root, "Root", GetRootComponent());
	Helpers::CreateComponent(this, &Cube, "Cube", Root);
	Helpers::CreateComponent<UArrowComponent>(this, &PlayerSpawnTransform, "PlayerSpawnTransform", Root);

}

void ALobbyPlatforms::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALobbyPlatforms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyPlatforms::SpawnCharacter(ALobbyController* InLobbyController)
{
	LobbyController = InLobbyController;

	if (CurrentCharacter)
	{
		CurrentCharacter->Destroy();
	}

	FTransform ArrowTransform = PlayerSpawnTransform->GetComponentTransform();
	FVector location = FVector(
		ArrowTransform.GetLocation().X + 1.0f,
		ArrowTransform.GetLocation().Y + 1.0f,
		ArrowTransform.GetLocation().Z + 84.0f);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(ArrowTransform.GetRotation());
	transform.SetScale3D(ArrowTransform.GetScale3D());

	FActorSpawnParameters SpawnParams;

	CurrentCharacter = GetWorld()->SpawnActor<ALobbyCharacter>(LobbyCharacter_Class, transform, SpawnParams);

}

void ALobbyPlatforms::ClearCharacter()
{
	LobbyController = nullptr;

	if (CurrentCharacter)
	{
		CurrentCharacter->Destroy();
		CurrentCharacter = nullptr;
	}
}

