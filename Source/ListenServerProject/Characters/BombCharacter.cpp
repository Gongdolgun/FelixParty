#include "Characters/BombCharacter.h"
#include "Utilites/Helpers.h"
#include "Camera/CameraComponent.h"
#include "Components/ZoomComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/MoveComponent.h"
#include "SpawnActor/Bomb.h"
#include "GameModes/BombGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Camera/CameraActor.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameState/DefaultGameState.h"
#include "Net/UnrealNetwork.h"
#include "SpawnActor/Restraint.h"
#include "SpawnActor/Wall.h"
#include "SpawnActor/TargetDecal.h"
#include "Widgets/TargetAim.h"

ABombCharacter::ABombCharacter()
{
	Helpers::CreateActorComponent<UZoomComponent>(this, &Zoom, "Zoom");
	Helpers::CreateComponent<UCameraComponent>(this, &TargetAimCamera, "TargetAimCamera", RootComponent);
	Helpers::CreateComponent(this, &BombSphereComponent, "BombLocation", GetCapsuleComponent());

	HandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HandSphere"));
	HandSphere->InitSphereRadius(20.0f);
	HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HandSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

	BombSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABombCharacter::BeginPlay()
{
	Super::BeginPlay();

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
			TargetDecal = GetWorld()->SpawnActor<ADecalActor>(DecalClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (TargetDecal)
			{
				TargetDecal->SetActorHiddenInGame(true);
			}
		}
	}
}

void ABombCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	ABombGameMode* BombGameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode());
	ABombCharacter* Attacker = Cast<ABombCharacter>(InActor);

	if (BombGameMode != nullptr && Attacker != nullptr)
	{
		BombGameMode->HitAttachBomb(this);
	}
}

void ABombCharacter::OnCollision()
{
	Super::OnCollision();

	HandSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABombCharacter::OffCollision()
{
	Super::OffCollision();

	HandSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABombCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetDecal != nullptr)
	{
		if (bIsAim)
		{
			FVector StartLocation = Camera->GetComponentLocation();
			FVector EndLocation = Camera->GetForwardVector() * 10000.f;

			TArray<AActor*> Ignore;
			Ignore.Add(this);
			FHitResult HitResult;

			// linetrace for decal
			if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery1, false, Ignore, EDrawDebugTrace::Type::None, HitResult, true))
			{
				TargetDecal->SetActorHiddenInGame(false);
				TargetDecal->SetActorLocation(HitResult.Location);
			}
		}

		else
		{
			TargetDecal->SetActorHiddenInGame(true);
		}
	}

	if (CurrentWallCoolTime > 0)
	{
		CurrentWallCoolTime = UKismetMathLibrary::FClamp(CurrentWallCoolTime - DeltaTime, 0, MaxWallCooltime);
	}
}

void ABombCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &ABombCharacter::Action);
		EnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Completed, this, &ABombCharacter::HandleAction);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, this, &ABombCharacter::SetZooming);
	}
}

void ABombCharacter::Action()
{
	Super::Action();

	if (GetCurrentMontage() == nullptr)
	{
		if (HasAuthority())
		{
			MulticastAttack();
		}
		else
		{
			ServerAttack();
		}
	}
}

void ABombCharacter::HandleAction()
{
	if (!bIsAim)
	{
		bIsAim = true;
	}

	else
	{
		if (TargetDecal && CurrentWallCoolTime == 0)
		{
			CurrentWallCoolTime = MaxWallCooltime;
			PlayMontage(Wall_Montage);
			ServerSpawnWall(TargetDecal->GetActorLocation(), GetActorRotation());

			TargetDecal->SetActorHiddenInGame(true);
			bIsAim = false; 
		}

		else if (CurrentWallCoolTime > 0)
		{
			TargetDecal->SetActorHiddenInGame(true);
			bIsAim = false;
		}
	}
}

void ABombCharacter::DeadEvent_NMC_Implementation()
{
	MoveComponent->CanMove = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	FVector ImpulseDirection = GetActorForwardVector() * 3000.f;
	ImpulseDirection.Z = 8000.f;

	GetMesh()->AddImpulse(ImpulseDirection, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABombCharacter::ServerAttack_Implementation()
{
	MulticastAttack();
}

void ABombCharacter::MulticastAttack_Implementation()
{
	PlayMontage(Attack_Montage);
}

void ABombCharacter::ServerSpawnWall_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if (TargetDecal)
	{
		GetWorld()->SpawnActor<AActor>(WallClass, Location, Rotation);
	}
}

void ABombCharacter::ServerSpawnRestraint_Implementation(const FVector& Location, const FRotator& Rotation)
{
	GetWorld()->SpawnActor<AActor>(RestraintClass, Location, Rotation);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

void ABombCharacter::OnSphereBeginOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IIDamage* HittedCharacter = Cast<IIDamage>(OtherActor);

	if (HittedCharacter != nullptr && HittedCharacter != this && bBomb == true)
	{
		HittedCharacter->Hit(this, FHitData());
	}
}

void ABombCharacter::SetZooming(const FInputActionValue& Value)
{
	float InValue = Value.Get<float>(); // 입력 액션에서 float 값을 가져옴

	InValue = -InValue;

	Zoom->SetZoomValue(InValue);
}

void ABombCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버에서 클라이언트로 값 전송
	DOREPLIFETIME(ThisClass, bBomb);
}

void ABombCharacter::ChangeSpeed()
{
	if (bBomb)
	{
		if (HasAuthority())
		{
			ChangeSpeed_NMC(BaseMovementSpeed * BombMovementSpeed);
		}

		else
		{
			ChangeSpeed_Server(BaseMovementSpeed * BombMovementSpeed);
		}
	}

	else
	{
		if (HasAuthority())
		{
			ChangeSpeed_NMC(BaseMovementSpeed);
		}

		else
		{
			ChangeSpeed_Server(BaseMovementSpeed);
		}
	}
}

void ABombCharacter::ChangeSpeed_Server_Implementation(float InSpeed)
{
	ChangeSpeed_NMC(InSpeed);
}

void ABombCharacter::ChangeSpeed_NMC_Implementation(float InSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = InSpeed;
}

void ABombCharacter::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (Montage && AnimInstance)
	{
		AnimInstance->Montage_Play(Montage);
	}
}
