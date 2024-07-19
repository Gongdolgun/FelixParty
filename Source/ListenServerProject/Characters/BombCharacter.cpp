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
	TopDownSpringArm->SetUsingAbsoluteRotation(true); // ���� ȸ���� ���
	TopDownSpringArm->TargetArmLength = 1000.f;
	TopDownSpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	TopDownSpringArm->bUsePawnControlRotation = false;
	TopDownSpringArm->bDoCollisionTest = false;

	TopDownCamera->SetupAttachment(TopDownSpringArm, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->SetUsingAbsoluteRotation(true); // ���� ȸ���� ���
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
			CLog::Log(*spawnLocation.ToString());

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

			Bomb->StartCountdown();

			// ��� Ŭ���̾�Ʈ���� ��ź ���� ������ ����
			MultiSpawnBomb(spawnBomb);
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

					AttackerCharacter->Bomb = nullptr;
					AttackerCharacter->bBomb = false;

					// ��ź ��ġ �� ���� ������Ʈ
					if (ABombGameMode* gameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode()))
					{
						gameMode->BombHolderController = HitCharacter->GetController();
					}

					// Ŭ���̾�Ʈ���� ������Ʈ ���� ����
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

	// �������� Ŭ���̾�Ʈ�� �� ����
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



