#include "Actors/OnlyUp/DeadCollision.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameModes/OnlyUpGameMode.h"

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
    
    if (character)
    {
        AOnlyUpGameMode* onlyUpGameMode = Cast<AOnlyUpGameMode>(GetWorld()->GetAuthGameMode());
        if (onlyUpGameMode == nullptr) return;
        
        SpawnIndex = character->GetSpawnIndex();
        FName TagToCheck = FName(*FString::Printf(TEXT("%d"), SpawnIndex));

        for (AActor* PlayerStart : PlayerStarts)
        {
            if (PlayerStart->ActorHasTag(TagToCheck)) 
            {
                FTransform transform;
                transform.SetLocation(PlayerStart->GetActorLocation());
                transform.SetRotation(FQuat(PlayerStart->GetActorForwardVector().Rotation()));

                character->RespawnPlayer(transform);

                break; 
            }
        }
    }
}
