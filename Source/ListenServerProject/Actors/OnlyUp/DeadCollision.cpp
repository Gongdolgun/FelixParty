#include "Actors/OnlyUp/DeadCollision.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "GameFramework/PlayerStart.h"

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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	
}

void ADeadCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeadCollision::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AOnlyUpCharacter* character = Cast<AOnlyUpCharacter>(OtherActor);

    if (character && (OtherActor != this))
    {
        int32 SpawnIndex = character->GetSpawnIndex();

        for (AActor* PlayerStart : PlayerStarts)
        {
            FName TagToCheck = FName(*FString::Printf(TEXT("%d"), SpawnIndex));

            if (PlayerStart->ActorHasTag(TagToCheck))
            {
                FVector SpawnLocation = PlayerStart->GetActorLocation();
                character->PlayerMaterialEventOnSpawn();
                character->SetActorLocation(SpawnLocation);
            }
        }
    }
}
