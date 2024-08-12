#include "Characters/BombCharacter.h"
#include "Utilites/Helpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "SpawnActor/Bomb.h"
#include "GameModes/BombGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Camera/CameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpawnActor/Restraint.h"
#include "SpawnActor/Wall.h"

ABombCharacter::ABombCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	HandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HandSphere"));
	HandSphere->InitSphereRadius(20.0f);
	HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	Bomb = nullptr;

	LastWallSpawnTime = -WallCoolTime;
	LastRestraintSpawnTime = -RestraintCoolTime;

}

void ABombCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABombCharacter>(GetOwner());

	// Scene���� ī�޶� ���� ã��
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);
	if (CameraActors.Num() > 0)
	{
		CameraActor = Cast<ACameraActor>(CameraActors[0]);
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && CameraActor)
	{
		PlayerController->SetViewTarget(CameraActor); // ���� ī�޶�� �� ��ȯ
	}

	// Hand_R_Sphere�� SphereComponent ����
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		HandSphere->AttachToComponent(MeshComp, AttachmentRules, TEXT("Hand_R_Sphere"));
	}

	HandSphere->OnComponentBeginOverlap.AddDynamic(this, &ABombCharacter::OnSphereBeginOverlap);

}

void ABombCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bBomb && Bomb)
	{
		// �������� ��ź�� ��ġ�� ĳ������ �Ӹ� ���� ��� ������Ʈ
		FVector spawnLocation = GetActorLocation() + FVector(0, 0, 200);

		// ��ġ�� ����� ��쿡�� ������Ʈ
		if (Bomb->GetActorLocation() != spawnLocation)
		{
			Bomb->SetActorLocation(spawnLocation);
			Bomb->BombLocation = spawnLocation; // ������ ��ġ �Ӽ�
			//CLog::Log(*spawnLocation.ToString());

			// ��ġ ������Ʈ�� Ŭ���̾�Ʈ�� �˸�
			Bomb->OnRep_UpdateBombLocation();
		}
	}
}

void ABombCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &ABombCharacter::Action);
		EnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Started, this, &ABombCharacter::HandleAction);

	}

}

void ABombCharacter::Action()
{
	Super::Action();

	if (HasAuthority())
	{
		MulticastAttack();
	}

	else
	{
		ServerAttack();
	}
}

void ABombCharacter::HandleAction()
{
	if (bBomb && Bomb)
	{
		ServerPlayRestraint();
	}

	else if (!bBomb && !Bomb)
	{
		ServerPlayWall();
	}
}

void ABombCharacter::Attack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Attack_Montage);
	}
}

void ABombCharacter::ServerAttack_Implementation()
{
	MulticastAttack();
}

void ABombCharacter::MulticastAttack_Implementation()
{
	Attack();
}

void ABombCharacter::ServerPlayWall_Implementation()
{
	MultiPlayWall();
}

void ABombCharacter::MultiPlayWall_Implementation()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Wall_Montage);
	}
}

void ABombCharacter::ServerSpawnWall_Implementation()
{
	float currentTime = GetWorld()->GetTimeSeconds();

	if (currentTime - LastWallSpawnTime < WallCoolTime)
	{
		return;
	}

	if (bBomb)
	{
		return;;
	}

	FActorSpawnParameters params;
	params.Owner = this;

	FVector location = this->GetActorLocation() + this->GetActorForwardVector() * Rate;
	location.Z -= 90.0f;
	FRotator rotation = FVector(this->GetActorForwardVector()).Rotation();
	FTransform transform = UKismetMathLibrary::MakeTransform(location, rotation, FVector(1, 1, 1));

	if (WallClass)
	{
		this->GetWorld()->SpawnActor<AActor>(WallClass, transform, params);

		LastWallSpawnTime = currentTime;
	}
}

void ABombCharacter::ServerPlayRestraint_Implementation()
{
	MultiPlayRestraint();
}

void ABombCharacter::MultiPlayRestraint_Implementation()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Restraint_Montage);
	}
}

void ABombCharacter::ServerSpawnRestraint_Implementation()
{
	float currentTime = GetWorld()->GetTimeSeconds();

	if (currentTime - LastRestraintSpawnTime < RestraintCoolTime)
	{
		return; // ��Ÿ���� �����ִ� ���
	}

	if (!bBomb)
	{
		return; 
	}

	if (!bIsSpawningRestraint) // �ߺ� ȣ�� ����
	{
		bIsSpawningRestraint = true;
		FActorSpawnParameters params;
		params.Owner = this;

		FVector location = this->GetActorLocation() + this->GetActorForwardVector() * 200;
		FRotator rotation = FVector(this->GetActorForwardVector()).Rotation();
		FTransform transform = UKismetMathLibrary::MakeTransform(location, rotation, FVector(1, 1, 1));

		if (RestraintClass)
		{
			this->GetWorld()->SpawnActor<AActor>(RestraintClass, transform, params);

			LastRestraintSpawnTime = currentTime;

		}

		// ���� �� �÷��� ����
		GetWorld()->GetTimerManager().SetTimer(ResetSpawnFlagHandle, [this]()
			{
				bIsSpawningRestraint = false;
			}, 1.0f, false); // 1�� �Ŀ� ���� (�ʿ信 ���� ����)
	}
}

// �������� ��ź�� ����
void ABombCharacter::ServerSpawnBomb_Implementation(TSubclassOf<ABomb> BombSpawn)
{
	// ��ź�� �̹� ������ ���°� �ƴϿ�����
	if (BombSpawn && !bBomb)
	{
		FVector spawnLocation = GetActorLocation() + FVector(0, 0, 200);
		FRotator spawnRotation = FRotator::ZeroRotator;

		// ��ź�� ���忡 ����
		ABomb* spawnBomb = GetWorld()->SpawnActor<ABomb>(BombSpawn, spawnLocation, spawnRotation);
		if (spawnBomb)
		{
			// ������ ��ź�� ĳ���Ϳ� ����
			spawnBomb->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			Bomb = spawnBomb;
			bBomb = true; // ��ź ���� ���·� ����(����)

			// ��� Ŭ���̾�Ʈ���� ��ź ���� ������ ����
			MultiSpawnBomb(spawnBomb);

			GetCharacterMovement()->MaxWalkSpeed = GetCurrentMovementSpeed();

			ResetBomb();
		}
	}
}

// ��� Ŭ���̾�Ʈ���� ��ź�� ����, ����ȭ
void ABombCharacter::MultiSpawnBomb_Implementation(ABomb* SpawnBomb)
{
	if (SpawnBomb && !HasAuthority())
	{
		SpawnBomb->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		Bomb = SpawnBomb;
		bBomb = true; // ��ź ���� ���·� ����(Ŭ��)

		GetCharacterMovement()->MaxWalkSpeed = GetCurrentMovementSpeed();
	}
}

void ABombCharacter::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && bAttack)
	{
		if (ABombCharacter* HitActor = Cast<ABombCharacter>(OtherActor))
		{
			OnAttackSuccess(this, HitActor);
		}
	}
}

void ABombCharacter::OnAttackSuccess(ACharacter* Attacker, ACharacter* HitActor)
{
	if (ABombCharacter* AttackerCharacter = Cast<ABombCharacter>(Attacker))
	{
		if (ABombCharacter* HitCharacter = Cast<ABombCharacter>(HitActor))
		{
			if (AttackerCharacter->bBomb && AttackerCharacter->Bomb)
			{
				ABomb* AttackerBomb = AttackerCharacter->Bomb;

				if (AttackerBomb)
				{
					GetWorld()->GetTimerManager().ClearTimer(AttackerCharacter->BombTimerHandle);

					AttackerBomb->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

					FVector spawnLocation = HitCharacter->GetActorLocation() + FVector(0, 0, 200);
					AttackerBomb->SetActorLocation(spawnLocation);
					AttackerBomb->BombLocation = spawnLocation; // BombLocation ������Ʈ

					CLog::Log(*AttackerBomb->BombLocation.ToString());

					AttackerBomb->AttachToActor(HitCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					HitCharacter->Bomb = AttackerBomb;
					HitCharacter->bBomb = true;

					HitCharacter->GetCharacterMovement()->MaxWalkSpeed = HitCharacter->GetCurrentMovementSpeed();

					AttackerCharacter->Bomb = nullptr;
					AttackerCharacter->bBomb = false;

					AttackerCharacter->GetCharacterMovement()->MaxWalkSpeed = AttackerCharacter->GetCurrentMovementSpeed();

					// ��ź ��ġ �� ���� ������Ʈ
					if (ABombGameMode* gameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
					{
						gameMode->BombHolderController = HitCharacter->GetController();
					}

					// Ŭ���̾�Ʈ���� ������Ʈ ���� ����
					HitCharacter->Bomb->OnRep_UpdateBombLocation();

					GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &ABombCharacter::EnableCollision, 1.0f, false);

					DisableCollision();

					HitCharacter->Bomb->CountdownSound = NewCountdownSound;

					HitCharacter->Bomb->StartCountdown();
					//HitCharacter->Bomb->MultiStartCountdown();

					// ��鸲 ȿ�� �ʱ�ȭ
					HitCharacter->Bomb->ResetShakeEffect();

					HitCharacter->ResetBomb();

				}
			}
		}
	}
}

void ABombCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �������� Ŭ���̾�Ʈ�� �� ����
	DOREPLIFETIME(ABombCharacter, bBombReplicate);
	DOREPLIFETIME(ABombCharacter, bBombReplicateMovement);
	DOREPLIFETIME(ABombCharacter, BombLocation);
	DOREPLIFETIME(ABombCharacter, bIsDead);

}

void ABombCharacter::EnableCollision()
{
	HandSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ABombCharacter::DisableCollision()
{
	HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABombCharacter::BombExplosion()
{
	if (bBomb && Bomb)
	{
		// �������� Dead �Լ��� ���� ȣ��
		if (HasAuthority())
		{
			Dead();
		}
	}
}

void ABombCharacter::ResetBomb()
{
	// ���� Ÿ�̸Ӱ� ������ ����
	GetWorld()->GetTimerManager().ClearTimer(BombTimerHandle);

	// ���ο� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(BombTimerHandle, this, &ABombCharacter::BombExplosion, 20.0f, false);
}

void ABombCharacter::PlayDead()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Dead_Montage);
	}
}

void ABombCharacter::Dead()
{
	if (HasAuthority())
	{
		bIsDead = true; // ��� ���·� ����
		MultiDead();    // ��Ƽ�÷��̾�� ȣ��
	}
}

float ABombCharacter::GetCurrentMovementSpeed() const
{
	if (bBomb)
	{
		return BaseMovementSpeed * BombMovementSpeed;
	}
	return BaseMovementSpeed;
}

void ABombCharacter::MultiDestroyCharacter_Implementation()
{
	Destroy();
}

void ABombCharacter::MultiDead_Implementation()
{
	if (Bomb)
	{
		Bomb->Explosion();
	}

	PlayDead();

	// ĳ���͸� ��Ȱ��ȭ�ϰ� ���ӿ��� ����
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	bBomb = false; // ��ź ���� ���� ����
	Bomb = nullptr; // ��ź ���� ����

	// GameMode���� ĳ���Ͱ� �׾����� �˸�
	if (ABombGameMode* GameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnPlayerDead(this);
	}

}
