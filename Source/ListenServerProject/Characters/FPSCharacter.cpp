#include "Characters/FPSCharacter.h"
#include "AnimInstance_DefaultCharacter.h"
#include "Global.h"
#include "Components/MoveComponent.h"
#include "Controllers/DefaultController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/FPSGameMode.h"
#include "GameState/DefaultGameState.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon.h"

AFPSCharacter::AFPSCharacter()
{
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	HP = MaxHP;
	CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AFPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (canRun)
		CurrentSpeed = FMath::Lerp(CurrentSpeed, 800.f, 2 * DeltaSeconds);

	else
		CurrentSpeed = 500.f;
		//CurrentSpeed = FMath::Lerp(CurrentSpeed, 500.f, 2 * DeltaSeconds);

	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

void AFPSCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	if (HasAuthority())
	{
		HP = UKismetMathLibrary::Clamp(HP - InHitData.Damage, 0, MaxHP);

		if(HP == 0)
		{
			// Ragdoll 처리, 장착중인 무기 Destroy
			Dead_NMC();

			// Attacker에게 점수 추가
			ADefaultCharacter* Attacker = Cast<ADefaultCharacter>(InActor);
			AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			SetSpeed(isRun);
			if(Attacker != nullptr && FPSGameMode != nullptr)
			{
				ADefaultGameState* DefaultGameState = Cast<ADefaultGameState>(FPSGameMode->GetGameState<ADefaultGameState>());
				ADefaultController* AttackerController = Cast<ADefaultController>(Attacker->GetController());
				if(DefaultGameState != nullptr && AttackerController != nullptr)
				{
					FString AttackerName = AttackerController->GetPlayerState<APlayerState>()->GetPlayerName();
					DefaultGameState->UpdatePlayerScore(AttackerName, 20);
				}
			}

			// Game Mode에 리스폰 요청
			GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RespawnCharacter, Respawn_time, false);
		}
	}
}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ThisClass::SetSpeed, true);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ThisClass::SetSpeed, false);
	}
}

void AFPSCharacter::Action()
{
	if (WeaponComponent != nullptr && MoveComponent->CanMove)
	{
		isAction = true;
		SetSpeed(isRun);
		WeaponComponent->Begin_Fire();
	}
}

void AFPSCharacter::End_Action()
{
	if (WeaponComponent != nullptr && MoveComponent->CanMove)
	{
		isAction = false;
		SetSpeed(isRun);
		WeaponComponent->End_Fire();
	}
}

void AFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HP);
	DOREPLIFETIME(ThisClass, canRun);
	DOREPLIFETIME(ThisClass, CurrentSpeed);
}

void AFPSCharacter::SeperateServer(FWeaponData WeaponData, FHitData HitData)
{
	// 서버라면 Linetrace 이벤트, 아니면 서버에서 이벤트 실행
	if (HasAuthority())
		LineTrace(WeaponData, HitData);

	else
		LineTrace_Server(WeaponData, HitData);
}

void AFPSCharacter::LineTrace(FWeaponData WeaponData, FHitData HitData)
{
	UCameraComponent* camera = Camera;
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, WeaponData.RecoilAngle);
	FVector end = transform.GetLocation() + direction * WeaponData.HitDistance;

	TArray<AActor*> ignores;
	ignores.Add(this);

	FHitResult hitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTyps;
	TEnumAsByte<EObjectTypeQuery> CharacterMesh = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECC_WorldDynamic);
	ObjectTyps.Add(CharacterMesh);
	ObjectTyps.Add(WorldStatic);
	ObjectTyps.Add(WorldDynamic);

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTyps, false, ignores, EDrawDebugTrace::None, hitResult, true))
	{
		ADefaultCharacter* HittedCharacter = Cast<ADefaultCharacter>(hitResult.GetActor());

		// 헤드 맞추면 데미지 100, 아니면 HitData의 Damage
		if (HittedCharacter != nullptr)
		{
			if (hitResult.BoneName == "head")
			{
				FHitData HeadShotData = HitData;
				HeadShotData.Damage = 100.f;

				HittedCharacter->Hit(this, HeadShotData);
			}

			else
				HittedCharacter->Hit(this, HitData);
		}
	}

	FireEvent_NMC(direction, hitResult);
}

void AFPSCharacter::SetSpeed(bool InIsRun)
{
	isRun = InIsRun;

	if (!isAction && !isAim && isRun)
		SetSpeed_Server(true);

	else
		SetSpeed_Server(false);
}

void AFPSCharacter::SetSpeed_Server_Implementation(bool InCanRun)
{
	canRun = InCanRun;
}

void AFPSCharacter::RespawnCharacter()
{
	AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (FPSGameMode != nullptr)
		FPSGameMode->RespawnPlayer(GetController());

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);

	Destroy();
}

void AFPSCharacter::Dead_NMC_Implementation()
{
	MoveComponent->CanMove = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	if(WeaponComponent != nullptr && WeaponComponent->CurWeapon != nullptr)
		WeaponComponent->CurWeapon->Destroy();
}

void AFPSCharacter::LineTrace_Server_Implementation(FWeaponData WeaponData, FHitData HitData)
{
	LineTrace(WeaponData, HitData);
}

void AFPSCharacter::FireEvent_NMC_Implementation(FVector direction, FHitResult HitResult)
{
	if (WeaponComponent->CurWeapon != nullptr)
		WeaponComponent->CurWeapon->Fire_Event(direction, HitResult);
}