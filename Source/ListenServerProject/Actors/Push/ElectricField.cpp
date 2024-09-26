#include "Actors/Push/ElectricField.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/BoxComponent.h"
#include "GameState/PushGameState.h"

AElectricField::AElectricField()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateComponent<UCapsuleComponent>(this, &RingCapsule, "RingCapsule");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &RingMesh, "RingMesh", RingCapsule);

	RingMesh->SetCollisionProfileName("OverlapAllDynamic");
	
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

		SetFieldRadius(FMath::Clamp(Newtime * CapsuleRadius, 10.0f, CapsuleRadius));
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
	if (character == nullptr) return;

	OverlappedCharacters.AddUnique(character);

	for (ADefaultCharacter* OverlapCharacter : OverlappedCharacters)
	{
		GetWorld()->GetTimerManager().SetTimer(DotTimerHandle, [this, OverlapCharacter]()
			{
				ApplyDamage(OverlapCharacter);
			}, DotInterval, true);
	}
	
}

void AElectricField::ApplyDamage(ADefaultCharacter* InCharacter)
{
	HitData.Damage = DotDamage;

	InCharacter->Hit(this, HitData);
}

void AElectricField::SetFieldRadius(float InRadius)
{
	RingCapsule->SetWorldScale3D(FVector(InRadius, InRadius, 50.0f));


}


