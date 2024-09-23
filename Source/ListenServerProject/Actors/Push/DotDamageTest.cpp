#include "Actors/Push/DotDamageTest.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/BoxComponent.h"

ADotDamageTest::ADotDamageTest()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent(this, &BoxCollision, "Collision", GetRootComponent());
}

void ADotDamageTest::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnComponentEndOverlap);
}

void ADotDamageTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADotDamageTest::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADefaultCharacter* character = Cast<ADefaultCharacter>(OtherActor);
	if (character == nullptr) return;

	GetWorld()->GetTimerManager().SetTimer(DotTimerHandle, [this, character]()
		{
			ApplyDamage(character); 
		}, DotInterval, true);

}

void ADotDamageTest::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADefaultCharacter* character = Cast<ADefaultCharacter>(OtherActor);
	if (character == nullptr) return;

	GetWorld()->GetTimerManager().ClearTimer(DotTimerHandle);
}

void ADotDamageTest::ApplyDamage(ADefaultCharacter* InCharacter)
{
	HitData.Damage = DotDamage;

	InCharacter->Hit(this, HitData);
}
