#include "Actors/Push/PushRespawner.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

APushRespawner::APushRespawner()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<UBoxComponent>(this, &Collision, "StaticMesh", GetRootComponent());

}

void APushRespawner::BeginPlay()
{
	Super::BeginPlay();

}

void APushRespawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SpawnLocation);
}

void APushRespawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APushRespawner::GetSpawnCollisionRandomPoint()
{
	FVector extent = Collision->GetScaledBoxExtent();
	SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), extent);

	return SpawnLocation;
}


