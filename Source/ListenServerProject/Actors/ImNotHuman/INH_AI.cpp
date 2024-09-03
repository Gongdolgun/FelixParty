#include "Actors/ImNotHuman/INH_AI.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Characters/INHCharacter.h"
#include "Controllers/DefaultController.h"
#include "GameState/INHGameState.h"

AINH_AI::AINH_AI()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AINH_AI::Hit(AActor* InActor, const FHitData& InHitData)
{
	AINHCharacter* Attacker = Cast<AINHCharacter>(InActor);

	if (Attacker != nullptr)
	{
		ADefaultController* AttackerController = Cast<ADefaultController>(Attacker->GetController());
		AINHGameState* GameState = Cast<AINHGameState>(GetWorld()->GetAuthGameMode()->GetGameState<AINHGameState>());

		if (AttackerController != nullptr && GameState != nullptr)
		{
			FString AttackerName = AttackerController->GetPlayerState<APlayerState>()->GetPlayerName();
			GameState->SomeoneDeadEvent(AttackerName, FString("AI"));
		}

		if(StunMontage != nullptr)
			Attacker->PlayAnimMontage_NMC(StunMontage);

		FVector ImpulseDirection = Attacker->GetActorForwardVector() * 10000.f;
		Dead_NMC(ImpulseDirection);
	}
}

void AINH_AI::BeginPlay()
{
	Super::BeginPlay();
	
}

void AINH_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AINH_AI::Dead_NMC_Implementation(FVector InImpulseDirection)
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	InImpulseDirection.Z = 3000.f;

	GetMesh()->AddImpulse(InImpulseDirection, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
