#include "Actors/OnlyUp/DeadCollision.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"

ADeadCollision::ADeadCollision()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UBoxComponent>(this, &Collision, "Collision", Root);
}

void ADeadCollision::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnLocation, FoundActors);

	for (AActor* spawnActor : FoundActors)
	{
		SpawnActors.Add(spawnActor);
	}
}

void ADeadCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeadCollision::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlyUpCharacter* character = Cast<AOnlyUpCharacter>(OtherActor);

	if (character && (OtherActor != this))
	{
		if (SpawnActors.Num() > 0)
		{
			int32 index = character->GetSpawnIndex();

			character->SetActorLocation(SpawnActors[1]->GetActorLocation());
			character->PlayerMaterialEventOnSpawn();
		}
	}
}

