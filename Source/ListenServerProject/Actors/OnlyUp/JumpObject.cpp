#include "Actors/OnlyUp/JumpObject.h"
#include "Components/StaticMeshComponent.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJumpObject::AJumpObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Root);

	Helpers::CreateComponent<UBoxComponent>(this, &Collision, "Collision", StaticMesh);
}

void AJumpObject::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void AJumpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character && (OtherActor != this))
	{
		FVector LaunchVelocity = FVector(0.0f, 0.0f, 1300.0f);
		character->LaunchCharacter(LaunchVelocity, true, true);
		
	}
}

