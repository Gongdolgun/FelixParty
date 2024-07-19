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
#include "Net/UnrealNetwork.h"
#include "SpawnActor/Wall.h"

ABombCharacter::ABombCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	/*Helpers::CreateComponent<USpringArmComponent>(this, &TopDownSpringArm, "TopDownSpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<UCameraComponent>(this, &TopDownCamera, "TopDownSpringArm", TopDownSpringArm);*/

	HandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HandSphere"));
	HandSphere->InitSphereRadius(20.0f);
	HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	TopDownSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TopDownSpringArm"));
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownSpringArm->SetupAttachment(RootComponent);
	TopDownSpringArm->SetUsingAbsoluteRotation(true); // 절대 회전값 사용
	TopDownSpringArm->TargetArmLength = 1000.f;
	TopDownSpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	TopDownSpringArm->bUsePawnControlRotation = false;
	TopDownSpringArm->bDoCollisionTest = false;

	TopDownCamera->SetupAttachment(TopDownSpringArm, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->SetUsingAbsoluteRotation(true); // 절대 회전값 사용
	//SpringArm->TargetArmLength = 1000.f;
	//SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	//SpringArm->bUsePawnControlRotation = false;
	//SpringArm->bDoCollisionTest = false;

	//Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	//Camera->bUsePawnControlRotation = false;

	Bomb = nullptr;

}

void ABombCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ABombCharacter>(GetOwner());

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
			CLog::Log(*spawnLocation.ToString());

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
		EnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Started, this, &ABombCharacter::ServerPlayWall);
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

void ABombCharacter::Attack()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Attack_Montage);
	}
}

void ABombCharacter::MulticastAttack_Implementation()
{
	Attack();
}


void ABombCharacter::ServerAttack_Implementation()
{
	MulticastAttack();
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
	FActorSpawnParameters params;
	params.Owner = this;

	FVector location = this->GetActorLocation() + this->GetActorForwardVector() * Rate;
	location.Z += 50.0f;
	FRotator rotation = FVector(this->GetActorForwardVector()).Rotation();
	FTransform transform = UKismetMathLibrary::MakeTransform(location, rotation, FVector(1, 1, 1));

	if (WallClass)
	{
		this->GetWorld()->SpawnActor<AActor>(WallClass, transform, params);
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

			Bomb->StartCountdown();

			// 모든 클라이언트에게 폭탄 생성 정보를 전파
			MultiSpawnBomb(spawnBomb);
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

					AttackerCharacter->Bomb = nullptr;
					AttackerCharacter->bBomb = false;

					// 폭탄 위치 및 상태 업데이트
					if (ABombGameMode* gameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
					{
						gameMode->BombHolderController = HitCharacter->GetController();
					}

					// 클라이언트에게 업데이트 정보 전파
					HitCharacter->Bomb->OnRep_UpdateBombLocation();

					GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &ABombCharacter::EnableCollision, 1.0f, false);

					DisableCollision();

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
	DOREPLIFETIME(ABombCharacter, ElapseTime);

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
		CLog::Log(TEXT("Bomb exploded!"));

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
	GetWorld()->GetTimerManager().SetTimer(BombTimerHandle, this, &ABombCharacter::BombExplosion, 10.0f, false);
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
		MultiDead();
	}
}

void ABombCharacter::MultiDead_Implementation()
{
	if (Bomb)
	{
		Bomb->Explosion();
		Bomb->MultiPlayCountdownSound();
	}

	PlayDead();
}



