#include "Actors/Crash/DestroyObject.h"
#include "Global.h"
#include "Actors/Crash/CrashFloor.h"
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
	if (OtherActor != nullptr)
	{
		ACrashFloor* CrashFloor = Cast<ACrashFloor>(OtherActor);

		if (CrashFloor)
		{
			FVector particleScale = FVector(2.0f, 2.0f, 2.0f);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator::ZeroRotator, particleScale);
			CrashFloor->LifeCount();
		}
	}
}

void ADestroyObject::Shot()
{
	if (Owner)
	{
		FVector InForward = FQuat(Owner->GetControlRotation()).GetForwardVector();
		FVector InitialVelocity = InForward * Projectile->InitialSpeed;

		InitialVelocity.Z += 500.0f; // Z축에 대한 초기 속도

		Projectile->Velocity = InitialVelocity;
		Projectile->Activate();
	}
}