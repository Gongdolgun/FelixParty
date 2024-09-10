#include "Characters/BombCharacter.h"
#include "Utilites/Helpers.h"
#include "Camera/CameraComponent.h"
#include "Components/ZoomComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "SpawnActor/Bomb.h"
#include "GameModes/BombGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Camera/CameraActor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "SpawnActor/Restraint.h"
#include "SpawnActor/Wall.h"
#include "SpawnActor/TargetDecal.h"
#include "Widgets/TargetAim.h"

ABombCharacter::ABombCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateActorComponent<UZoomComponent>(this, &Zoom, "Zoom");
	Helpers::CreateComponent<UCameraComponent>(this, &TargetAimCamera, "TargetAimCamera", RootComponent);

	HandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HandSphere"));
	HandSphere->InitSphereRadius(20.0f);
	HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	Bomb = nullptr;

	bIsDecal = false;

	LastWallSpawnTime = -WallCoolTime;
	LastRestraintSpawnTime = -RestraintCoolTime;

}

void ABombCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABombCharacter>(GetOwner());

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		HandSphere->AttachToComponent(MeshComp, AttachmentRules, TEXT("Hand_R_Sphere"));
	}

	HandSphere->OnComponentBeginOverlap.AddDynamic(this, &ABombCharacter::OnSphereBeginOverlap);

	if (DecalClass)
	{
		if (!TargetDecal)
		{
			TargetDecal = GetWorld()->SpawnActor<ATargetDecal>(DecalClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (TargetDecal)
			{
				TargetDecal->SetActorHiddenInGame(true);
			}
		}
	}

	if (TargetAimClass)
	{
		TargetAimWidget = CreateWidget<UTargetAim>(GetWorld(), TargetAimClass);

		if (TargetAimWidget)
		{
			TargetAimWidget->AddToViewport();
			TargetAimWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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

	if (TargetDecal && !TargetDecal->IsHidden())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			FVector MouseLocation, MouseDirection;

			// ���콺 ��ġ�� ���� �������� ��ȯ
			if (PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
			{
				FHitResult HitResult;
				FCollisionQueryParams CollisionParams;
				CollisionParams.AddIgnoredActor(this);

				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, MouseLocation, MouseLocation + (MouseDirection * 10000.0f), ECC_Visibility, CollisionParams);

				if (bHit)
				{
					TargetDecal->UpdateDecalLocation(HitResult.Location, HitResult.ImpactNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f));
				}
			}
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
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ABombCharacter::SetZooming);
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
	if (CurrentActionState == EActionState::Dead) 
		return;

	if (IsInAction())
	{
		return; 
	} 

	if (TargetDecal)
	{
		if (!bIsDecal && !bBomb && !Bomb)
		{
			TargetDecal->SetActorHiddenInGame(false);
			bIsDecal = true;
		}

		else
		{
			FVector Location = TargetDecal->GetActorLocation();
			FRotator Rotation = FVector(this->GetActorForwardVector()).Rotation();

			ServerSpawnWall(Location, Rotation);
			TargetDecal->SetActorHiddenInGame(true);
			bIsDecal = false;
		}
	}

	if (TargetAimWidget)
	{
		if (bBomb && Bomb)
		{
			if (TargetAimWidget->GetVisibility() == ESlateVisibility::Hidden)
			{
				TargetAimWidget->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ServerSpawnRestraint();
				TargetAimWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ABombCharacter::Attack()
{
	if (CurrentActionState == EActionState::Dead) 
		return;

	if (IsInAction())
	{
		return;
	}

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
	if (CurrentActionState == EActionState::Dead) 
		return;

	MultiPlayWall();
}

void ABombCharacter::MultiPlayWall_Implementation()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Wall_Montage);
	}
}

void ABombCharacter::MultiSpawnWall_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if (WallClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FTransform Transform = UKismetMathLibrary::MakeTransform(Location, Rotation, FVector(1, 1, 1));
		GetWorld()->SpawnActor<AActor>(WallClass, Transform, SpawnParams);
	}
}

void ABombCharacter::ServerSpawnWall_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if (CurrentActionState == EActionState::Dead || !TargetDecal)
		return;

	float currentTime = GetWorld()->GetTimeSeconds();

	if (currentTime - LastWallSpawnTime < WallCoolTime)
	{
		return;
	}

	if (bBomb)
	{
		return;;
	}

	// MultiSpawnWall �Լ� ȣ���Ͽ� Ŭ���̾�Ʈ��� ������ Wall ����
	MultiSpawnWall(Location, Rotation);

	LastWallSpawnTime = currentTime;
}

void ABombCharacter::ServerPlayRestraint_Implementation()
{
	if (CurrentActionState == EActionState::Dead)
		return;

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

		FVector location = GetActorLocation() + GetControlRotation().Vector() * 300; // ī�޶��� ��ġ���� ������ ����
		FRotator rotation = GetControlRotation();

		if (RestraintClass)
		{
			ARestraint* restraint = this->GetWorld()->SpawnActor<ARestraint>(RestraintClass, location, rotation, params);

			if (restraint)
			{
				restraint->PlayerCharacter = this; 

				FVector launchDirection = GetControlRotation().Vector();
				launchDirection.Normalize();

				MultiSpawnRestraint(location, rotation, launchDirection * restraint->Projectile->InitialSpeed);

				LastRestraintSpawnTime = currentTime;
			}
		}

		// ���� �� �÷��� ����
		GetWorld()->GetTimerManager().SetTimer(ResetSpawnFlagHandle, [this]()
			{
				bIsSpawningRestraint = false;
			}, 1.0f, false); // 1�� �Ŀ� ���� (�ʿ信 ���� ����)
	}
}

void ABombCharacter::MultiSpawnRestraint_Implementation(const FVector& Location, const FRotator& Rotation, const FVector& Velocity)
{
	FActorSpawnParameters params;
	params.Owner = this;

	ARestraint* restraint = this->GetWorld()->SpawnActor<ARestraint>(RestraintClass, Location, Rotation, params);

	if (restraint)
	{
		restraint->PlayerCharacter = this; 
		restraint->Projectile->Velocity = Velocity; 
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

void ABombCharacter::PlaceWall()
{
	if (TargetDecal && WallClass)
	{
		// ��Į�� ��ġ�� ȸ�� ������ ����Ͽ� �� ����
		FVector SpawnLocation = TargetDecal->GetActorLocation();
		FRotator SpawnRotation = TargetDecal->GetActorRotation();

		// �� ����
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<AActor>(WallClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}

void ABombCharacter::SetActionState(EActionState NewState)
{
	CurrentActionState = NewState;
}

bool ABombCharacter::IsInAction() const
{
	return CurrentActionState == EActionState::InAction;
}

void ABombCharacter::SetZooming(const FInputActionValue& Value)
{
	float InValue = Value.Get<float>(); // �Է� �׼ǿ��� float ���� ������

	InValue = -InValue;

	Zoom->SetZoomValue(InValue);
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
		bIsDead = true;
		CurrentActionState = EActionState::Dead;
		MultiDead();
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

	CurrentActionState = EActionState::Dead;

	// GameMode���� ĳ���Ͱ� �׾����� �˸�
	if (ABombGameMode* GameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnPlayerDead(this);
	}

}
