#include "Actors/Spawner.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Net/UnrealNetwork.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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

	SpawnActor();
}

void ASpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, SpawnedActor);
}

void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ASpawner::SpawnActor()
{
	if (SpawnActorClass.Num() > 0 && HasAuthority())
	{
		int32 RandomInteger = UKismetMathLibrary::RandomIntegerInRange(0, SpawnActorClass.Num() - 1);
		if (SpawnActorClass[RandomInteger] != nullptr)
		{
			FVector Location = GetActorLocation() + SpawnLocation;

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass[RandomInteger], Location, FRotator::ZeroRotator, params);

			CreateRotatingMovementComponent();
		}
	}
}

void ASpawner::CreateRotatingMovementComponent()
{
	if (isRotate && SpawnedActor)
	{
		URotatingMovementComponent* RotatingMovementComponent = NewObject<URotatingMovementComponent>(SpawnedActor);
		if (RotatingMovementComponent)
		{
			RotatingMovementComponent->RegisterComponent();
			RotatingMovementComponent->SetIsReplicated(true);
			RotatingMovementComponent->SetUpdatedComponent(SpawnedActor->GetRootComponent());

			RotatingMovementComponent->RotationRate = RotateRate;
		}
	}
}
