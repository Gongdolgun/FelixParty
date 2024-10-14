#include "Actors/Push/ElectricField.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/BoxComponent.h"
#include "GameState/PushGameState.h"

AElectricField::AElectricField()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Helpers::CreateComponent<UStaticMeshComponent>(this, &RingMesh, "RingMesh");
    Helpers::CreateComponent<UCapsuleComponent>(this, &RingCapsule, "RingCapsule", RingMesh);

    RingMesh->SetCollisionProfileName("RingCapsule");

    //SetActorTickInterval(1.0f);
}

void AElectricField::BeginPlay()
{
    Super::BeginPlay();

    PushGameState = Cast<APushGameState>(GetWorld()->GetGameState());
    GamePlayTime = PushGameState->GamePlayTime;
    CapsuleRadius = GetActorScale().X;

    RingCapsule->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
    RingCapsule->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);
}

void AElectricField::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void AElectricField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PushGameState && PushGameState->GetGameStateType() == EGameStateType::GamePlay)
    {
        float Newtime = PushGameState->GamePlayTime / GamePlayTime;

        SetFieldRadius(FMath::Clamp(Newtime * CapsuleRadius, 20.0f, CapsuleRadius));
    }
}

void AElectricField::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (PushGameState->GetGameStateType() != EGameStateType::GamePlay) return;

    ADefaultCharacter* character = Cast<ADefaultCharacter>(OtherActor);
    if (character == nullptr) return;

    if (DotTimerHandle.IsValid())
        GetWorld()->GetTimerManager().ClearTimer(DotTimerHandle);

    if (OverlappedCharacters.Contains(character))
        OverlappedCharacters.Remove(character);
}

void AElectricField::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (PushGameState->GetGameStateType() != EGameStateType::GamePlay) return;

    ADefaultCharacter* character = Cast<ADefaultCharacter>(OtherActor);
    if (character)
    {
        OverlappedCharacters.AddUnique(character);

        for (ADefaultCharacter* OverlapCharacter : OverlappedCharacters)
        {
            TWeakObjectPtr<ADefaultCharacter> WeakCharacter = OverlapCharacter;

            GetWorld()->GetTimerManager().SetTimer(DotTimerHandle, [this, WeakCharacter]()
                {
                    if (WeakCharacter.IsValid())
                    {
                        ApplyDamage(WeakCharacter.Get());
                    }
                }, DotInterval, true);
        }
    }
}

void AElectricField::ApplyDamage(ADefaultCharacter* InCharacter)
{
    HitData.Damage = DotDamage;

    InCharacter->Hit(this, HitData);
}

void AElectricField::SetFieldRadius(float InRadius)
{
    RingCapsule->SetWorldScale3D(FVector(InRadius, InRadius, 80.0f));
    CurrFieldRadius = RingCapsule->GetScaledCapsuleRadius();
}