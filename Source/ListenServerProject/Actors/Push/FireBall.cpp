#include "Actors/Push/FireBall.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/IDamage.h"

AFireBall::AFireBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent(this, &Root, "Root");
	Helpers::CreateComponent(this, &Mesh, "Mesh", Root);
	Helpers::CreateComponent(this, &Collision, "Collision", Root);
	Helpers::CreateComponent(this, &Particle, "Particle", Root);
	Helpers::CreateActorComponent(this, &ProjectileComponent, "ProjectileComponent");

	bReplicates = true;
	Particle->SetIsReplicated(true);
	ProjectileComponent->InitialSpeed = 1000.0f;

}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.0f);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	IIDamage* HittedCharacter = Cast<IIDamage>(OtherActor);
	if (HittedCharacter == nullptr || character == nullptr) return;

	HitData.Damage = 40.0f;
	HitData.Launch = GetActorForwardVector() * 1500.0f;

	HittedCharacter->Hit(this, HitData);

	Destroyed();
}

void AFireBall::OnDestroy()
{
	
}

void AFireBall::Destroyed()
{
	Super::Destroyed();

	if (this == nullptr) return;

	if (Explosion)
	{
		FTransform transform;
		transform.SetLocation(GetActorLocation());
		transform.SetRotation(FQuat(GetActorRotation()));
		transform.SetScale3D(FVector(2.0f, 2.0f, 2.0f));

		UParticleSystemComponent* particleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
		particleComponent->SetIsReplicated(true);
	}

	Particle->SetActive(false);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

