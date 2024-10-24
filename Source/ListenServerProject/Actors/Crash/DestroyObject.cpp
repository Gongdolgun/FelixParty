#include "Actors/Crash/DestroyObject.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ADestroyObject::ADestroyObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Sphere);
	Helpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", StaticMesh);
	Helpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

}

void ADestroyObject::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADestroyObject::OnComponentBeginOverlap);

}

void ADestroyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

