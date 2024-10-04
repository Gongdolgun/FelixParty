#include "Actors/Push/ElectricField.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/BoxComponent.h"

AElectricField::AElectricField()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<UCapsuleComponent>(this, &RingCapsule, "RingCapsule");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &RingMesh, "RingMesh", RingCapsule);

	RingMesh->SetCollisionProfileName("OverlapAllDynamic");

	bReplicates = true;
	SetActorTickInterval(1.0f);
}

void AElectricField::BeginPlay()
{
	Super::BeginPlay();

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

}

void AElectricField::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADefaultCharacter* character = Cast<ADefaultCharacter>(OtherActor);
	if (character == nullptr) return;

	if (DotTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DotTimerHandle);

	if (OverlappedCharacters.Contains(character))
	{
		OverlappedCharacters.Remove(character);
	}
}

void AElectricField::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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
