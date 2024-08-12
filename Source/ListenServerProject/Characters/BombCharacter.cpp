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

	// Scene에서 카메라 액터 찾기
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);
	if (CameraActors.Num() > 0)
	{
		CameraActor = Cast<ACameraActor>(CameraActors[0]);
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && CameraActor)
	{
		PlayerController->SetViewTarget(CameraActor); // 기존 카메라로 뷰 전환
	}

	// Hand_R_Sphere에 SphereComponent 장착
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
		// 서버에서 폭탄의 위치를 캐릭터의 머리 위로 계속 업데이트
		FVector spawnLocation = GetActorLocation() + FVector(0, 0, 200);

		// 위치가 변경될 경우에만 업데이트
		if (Bomb->GetActorLocation() != spawnLocation)
		{
			Bomb->SetActorLocation(spawnLocation);
			Bomb->BombLocation = spawnLocation; // 복제된 위치 속성
			//CLog::Log(*spawnLocation.ToString());

			// 위치 업데이트를 클라이언트에 알림
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
		return; // 쿨타임이 남아있는 경우
	}

	if (!bBomb)
	{
		return; 
	}

	if (!bIsSpawningRestraint) // 중복 호출 방지
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

		// 스폰 후 플래그 리셋
		GetWorld()->GetTimerManager().SetTimer(ResetSpawnFlagHandle, [this]()
			{
				bIsSpawningRestraint = false;
			}, 1.0f, false); // 1초 후에 리셋 (필요에 따라 조정)
	}
}

// 서버에서 폭탄을 생성
void ABombCharacter::ServerSpawnBomb_Implementation(TSubclassOf<ABomb> BombSpawn)
{
	// 폭탄이 이미 생성된 상태가 아니여야함
	if (BombSpawn && !bBomb)
	{
		FVector spawnLocation = GetActorLocation() + FVector(0, 0, 200);
		FRotator spawnRotation = FRotator::ZeroRotator;

		// 폭탄을 월드에 생성
		ABomb* spawnBomb = GetWorld()->SpawnActor<ABomb>(BombSpawn, spawnLocation, spawnRotation);
		if (spawnBomb)
		{
			// 생성된 폭탄을 캐릭터에 부착
			spawnBomb->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			Bomb = spawnBomb;
			bBomb = true; // 폭탄 소유 상태로 변경(서버)

			// 모든 클라이언트에게 폭탄 생성 정보를 전파
			MultiSpawnBomb(spawnBomb);

			GetCharacterMovement()->MaxWalkSpeed = GetCurrentMovementSpeed();

			ResetBomb();
		}
	}
}

// 모든 클라이언트에서 폭탄을 부착, 동기화
void ABombCharacter::MultiSpawnBomb_Implementation(ABomb* SpawnBomb)
{
	if (SpawnBomb && !HasAuthority())
	{
		SpawnBomb->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		Bomb = SpawnBomb;
		bBomb = true; // 폭탄 소유 상태로 변경(클라)

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
					AttackerBomb->BombLocation = spawnLocation; // BombLocation 업데이트

					CLog::Log(*AttackerBomb->BombLocation.ToString());

					AttackerBomb->AttachToActor(HitCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

					HitCharacter->Bomb = AttackerBomb;
					HitCharacter->bBomb = true;

					HitCharacter->GetCharacterMovement()->MaxWalkSpeed = HitCharacter->GetCurrentMovementSpeed();

					AttackerCharacter->Bomb = nullptr;
					AttackerCharacter->bBomb = false;

					AttackerCharacter->GetCharacterMovement()->MaxWalkSpeed = AttackerCharacter->GetCurrentMovementSpeed();

					// 폭탄 위치 및 상태 업데이트
					if (ABombGameMode* gameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
					{
						gameMode->BombHolderController = HitCharacter->GetController();
					}

					// 클라이언트에게 업데이트 정보 전파
					HitCharacter->Bomb->OnRep_UpdateBombLocation();

					GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &ABombCharacter::EnableCollision, 1.0f, false);

					DisableCollision();

					HitCharacter->Bomb->CountdownSound = NewCountdownSound;

					HitCharacter->Bomb->StartCountdown();
					//HitCharacter->Bomb->MultiStartCountdown();

					// 흔들림 효과 초기화
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

	// 서버에서 클라이언트로 값 전송
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
		// 서버에서 Dead 함수를 직접 호출
		if (HasAuthority())
		{
			Dead();
		}
	}
}

void ABombCharacter::ResetBomb()
{
	// 기존 타이머가 있으면 정지
	GetWorld()->GetTimerManager().ClearTimer(BombTimerHandle);

	// 새로운 타이머 설정
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
		bIsDead = true; // 사망 상태로 변경
		MultiDead();    // 멀티플레이어에서 호출
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

	// 캐릭터를 비활성화하고 게임에서 제거
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	bBomb = false; // 폭탄 소유 상태 해제
	Bomb = nullptr; // 폭탄 참조 해제

	// GameMode에게 캐릭터가 죽었음을 알림
	if (ABombGameMode* GameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnPlayerDead(this);
	}

}
