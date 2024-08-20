#include "Actors/OnlyUp/SpawnLocation.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ASpawnLocation::ASpawnLocation()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UBoxComponent>(this, &Collision, "Collision", Root);

}

void ASpawnLocation::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void ASpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnLocation::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character && (OtherActor != this))
	{

	}
}
