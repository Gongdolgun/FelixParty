#include "Actors/Spawner.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	Helpers::CreateComponent(this, &Root, "Root");
	Helpers::CreateComponent(this, &Sphere, "Sphere", Root);
	Helpers::CreateComponent(this, &Mesh, "Mesh", Root);

	Sphere->SetRelativeLocation(FVector(0, 0, 60));
	Sphere->SetSphereRadius(60.f);
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);

	if(SpawnActorClass)
	{
		FVector Location = GetActorLocation() + SpawnLocation;

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, Location, FRotator::ZeroRotator, params);

		CreateRotatingMovementComponent();
	}
}

void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ASpawner::CreateRotatingMovementComponent()
{
	if (isRotate && SpawnedActor)
	{
		URotatingMovementComponent* RotatingMovementComponent = NewObject<URotatingMovementComponent>(SpawnedActor);
		if (RotatingMovementComponent)
		{
			RotatingMovementComponent->RegisterComponent();
			RotatingMovementComponent->SetUpdatedComponent(SpawnedActor->GetRootComponent());

			RotatingMovementComponent->RotationRate = RotateRate;
		}
	}
}