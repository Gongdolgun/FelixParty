#include "Actors/OnlyUp/SavePoint.h"
#include "Global.h"
#include "Characters/OnlyUpCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASavePoint::ASavePoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UBoxComponent>(this, &Collision, "Collision", Root);
}

void ASavePoint::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASavePoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnlyUpCharacter* character = Cast<AOnlyUpCharacter>(OtherActor);

	if (character && (OtherActor != this))
	{
		character->SetSpawnIndex(SavePoint);

	}
}

