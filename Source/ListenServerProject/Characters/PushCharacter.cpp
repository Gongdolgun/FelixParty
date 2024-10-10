#include "Characters/PushCharacter.h"
#include "Global.h"
#include "Actors/Push/PushRespawner.h"
#include "Components/MoveComponent.h"
#include "Components/StateComponent.h"
#include "Controllers/DefaultController.h"
#include "Controllers/PushController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/PushGameMode.h"
#include "GameState/DefaultGameState.h"
#include "GameState/PushGameState.h"
#include "Net/UnrealNetwork.h"

APushCharacter::APushCharacter()
{
    Helpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "State");
    StateComponent->SetIsReplicated(true);

    SpringArm->bDoCollisionTest = true;

    bReplicates = true;
    HP = 100.0f;
    Attacker = nullptr;
}

void APushCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (MoveComponent)
        MoveComponent->EnableControlRotation();

    if (StateComponent)
        StateComponent->SetIdleMode();

    PushGameMode = Cast<APushGameMode>(GetWorld()->GetAuthGameMode());
    PushGameState = Cast<APushGameState>(GetWorld()->GetGameState());

}

void APushCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APushCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APushCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, Attacker);
}

void APushCharacter::Action()
{
    Super::Action();

    if (StateComponent->IsIdleMode() == false) return;
    if (GetCharacterMovement()->IsFalling() == true) return;

    StateComponent->SetActionMode();

    if (HasAuthority())
    {
        PlayActionMontage_NMC(AttackMontage);
    }

    else
    {
        PlayActionMontage_Server(AttackMontage);
    }
}

void APushCharacter::Jump()
{
    if (StateComponent->IsIdleMode())
    {
        Super::Jump();
    }
}

void APushCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
    Super::Hit(InActor, InHitData);

    if (StateComponent->IsDeadMode() == true) return;

    if (InActor->GetOwner() != nullptr)
    {
        APushCharacter* attacker = Cast<APushCharacter>(InActor->GetOwner());
        if (attacker != nullptr)
        {
            SetAttacker_Server(attacker);
        }

        if (Attacker)
        {
            APushController* attackerController = Cast<APushController>(Attacker->GetController());
            APushController* playerController = Cast<APushController>(GetController());

            if (attackerController && playerController)
            {
                attackerName = attackerController->GetPlayerState<APlayerState>()->GetPlayerName();
                playerName = playerController->GetPlayerState<APlayerState>()->GetPlayerName();

            }
        }
    }

    HP = UKismetMathLibrary::Clamp(HP - InHitData.Damage, 0, MaxHP);

    if (HP > 0.0f)
    {
        // 몽타주가 있을 때만
        if (InHitData.Montage)
        {
            StateComponent->SetHittedMode();

            if (HasAuthority())
                PlayActionMontage_NMC(InHitData.Montage);

            else
                PlayActionMontage_Server(InHitData.Montage);
        }

        LaunchCharacter(InHitData.Launch, true, false);
    }

    // 플레이어 사망
    else
    {
        FVector ImpulseDirection = InActor->GetActorForwardVector() * 1000.f;

        if (HasAuthority())
            Dead_NMC(ImpulseDirection);

        if (attackerName.IsEmpty() == false)
        {
            PushGameState->SomeoneDeadEvent(attackerName, playerName);
            PushGameState->UpdatePlayerScore(attackerName, 20);
        }

        // 랜덤 위치 캐릭터 스폰
        GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &APushCharacter::RespawnCharacter, 2.0f, false);

        //RespawnCharacter();
    }

}

void APushCharacter::SetAttacker_NMC_Implementation(APushCharacter* InCharacter)
{
    Attacker = InCharacter;
}

void APushCharacter::SetAttacker_Server_Implementation(APushCharacter* InCharacter)
{
    SetAttacker_NMC(InCharacter);
}

void APushCharacter::RespawnCharacter()
{
    if (PushGameMode)
        PushGameMode->RespawnPlayer(GetController());

    if (SpawnTimerHandle.IsValid())
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

    Destroy();
}

void APushCharacter::Dead_NMC_Implementation(FVector InImpulse)
{
    MoveComponent->CanMove = false;
    StateComponent->SetDeadMode();

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName("Ragdoll");

    InImpulse.Z = 500.f;
    GetMesh()->AddImpulse(InImpulse, NAME_None, true);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APushCharacter::SpawnObject_Server_Implementation(UClass* InClass, FTransform InTransform)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    GetWorld()->SpawnActor(InClass, &InTransform, SpawnParams);
}


void APushCharacter::PlayActionMontage_NMC_Implementation(UAnimMontage* InMontage)
{
    if (InMontage)
        PlayAnimMontage(InMontage, 1.35f);
}

void APushCharacter::PlayActionMontage_Server_Implementation(UAnimMontage* InMontage)
{
    if (InMontage)
        PlayActionMontage_NMC(InMontage);

}