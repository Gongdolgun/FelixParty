#include "SpawnActor/Restraint.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/BombCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARestraint::ARestraint()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Capsule);
	Helpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->ProjectileGravityScale = 0;
	Projectile->Activate();
	Projectile->bRotationFollowsVelocity = true;
	Projectile->bShouldBounce = false;

	Capsule->SetSimulatePhysics(false);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

void ARestraint::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABombCharacter>(GetOwner());

	Capsule->OnComponentHit.AddDynamic(this, &ARestraint::OnHit);
}

void ARestraint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARestraint::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	/*if (OtherActor && OtherActor != this)
	{
		ABombCharacter* BombCharacter = Cast<ABombCharacter>(OtherActor);

		if (BombCharacter)
		{
			if (BombCharacter->Bomb && BombCharacter->bBomb)
			{
				Destroy();
				return;
			}

			DisableMovement(BombCharacter);

			if (Particle)
			{
				FVector spawnLocation = BombCharacter->GetActorLocation() + FVector(0, 0, -50.0f);
				FRotator spawnRotation = BombCharacter->GetActorRotation();

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, spawnLocation, spawnRotation);
			}
		}

		Destroy();
	}*/

	if (OtherActor == this)
	{
		Destroy();

		return;
	}

	if (!OtherActor->IsA<ABombCharacter>())
	{
		Destroy();

		return;
	}

	ABombCharacter* BombCharacter = Cast<ABombCharacter>(OtherActor);
	if (BombCharacter)
	{
		if (BombCharacter->bBomb)
		{
			Destroy();
			return;
		}

		DisableMovement(BombCharacter);

		if (Particle)
		{
			FVector spawnLocation = BombCharacter->GetActorLocation() + FVector(0, 0, -50.0f);
			FRotator spawnRotation = BombCharacter->GetActorRotation();

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, spawnLocation, spawnRotation);
		}
	}

	Destroy();
}

void ARestraint::DisableMovement(ABombCharacter* TargetCharacter)
{
	ABombCharacter* HitCharacter = Cast<ABombCharacter>(TargetCharacter);
	if (HitCharacter)
	{
		// PlayerCharacter의 이동을 비활성화
		HitCharacter->GetCharacterMovement()->DisableMovement();

		// 5초 후에 PlayerCharacter의 이동을 다시 활성화하는 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, [HitCharacter]()
			{
				if (HitCharacter)
				{
					HitCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
				}
			}, 5.0f, false);
	}
}

void ARestraint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

