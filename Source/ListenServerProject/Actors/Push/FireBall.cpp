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
	ProjectileComponent->InitialSpeed = 2000.0f;
}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(4.0f);

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr) return;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProjectileComponent)
	{
		if (OwnerCharacter)
		{
			Object_Velocity = OwnerCharacter->GetActorForwardVector() * ProjectileComponent->InitialSpeed;

			ProjectileComponent->Velocity = UKismetMathLibrary::VInterpTo(ProjectileComponent->Velocity, Object_Velocity, DeltaTime, InterpSpeed);
			SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), Object_Rotation, DeltaTime, InterpSpeed));
		}
	}
	
}

void AFireBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	IIDamage* HittedCharacter = Cast<IIDamage>(OtherActor);

	if (OtherActor != OwnerCharacter)
	{
		if (HittedCharacter && character)
		{
			HitData.Damage = 5.0f;
			HitData.Launch = (GetActorForwardVector() * 1800.0f) + FVector(0.0f, 0.0f, 300.0f);

			HittedCharacter->Hit(this, HitData);
		}
		
		OnDestroy();

		return;
	}
}

void AFireBall::OnDestroy()
{
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
