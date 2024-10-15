#include "Actors/Crash/DestroyObject.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADestroyObject::ADestroyObject()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Sphere);
	Helpers::CreateComponent<UParticleSystem>(this, &Particle, "Particle", StaticMesh);
	Helpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

}

void ADestroyObject::BeginPlay()
{
	Super::BeginPlay();

}

void ADestroyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

