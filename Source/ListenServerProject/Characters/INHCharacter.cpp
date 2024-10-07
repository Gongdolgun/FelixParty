#include "Characters/INHCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Components/MoveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StateComponent.h"
#include "Controllers/DefaultController.h"
#include "Controllers/INHController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameModes/INHGameMode.h"
#include "GameState/DefaultGameState.h"
#include "GameState/INHGameState.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/INHMain.h"

AINHCharacter::AINHCharacter()
{
	Helpers::CreateComponent(this, &Sphere, "Sphere", GetCapsuleComponent());

	Helpers::CreateActorComponent(this, &StateComponent, "StateComponent");

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AINHCharacter::BeginPlay()
{
	Super::BeginPlay();

	int32 MaterialCount = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < MaterialCount; i++)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(i));
		if (!MaterialInstance)
			MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);

		if (MaterialInstance)
			MyMaterials.Add(MaterialInstance);
	}

	Sphere->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName("hand_r"));
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void AINHCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (canRun)
		CurrentSpeed = FMath::Lerp(CurrentSpeed, 500.f, 2 * DeltaSeconds);

	else
		CurrentSpeed = FMath::Lerp(CurrentSpeed, 230.f, 2 * DeltaSeconds);

	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

void AINHCharacter::Hit(AActor* InActor, const FHitData& InHitData)
{
	Super::Hit(InActor, InHitData);

	ADefaultCharacter* Attacker = Cast<ADefaultCharacter>(InActor);

	if(Attacker != nullptr)
	{
		ADefaultController* AttackerController = Cast<ADefaultController>(Attacker->GetController());
		AINHController* MyController = Cast<AINHController>(GetController());
		AINHGameState* GameState = Cast<AINHGameState>(GetWorld()->GetAuthGameMode()->GetGameState<AINHGameState>());

		if (AttackerController != nullptr && GameState != nullptr)
		{
			FString AttackerName = AttackerController->GetPlayerState<APlayerState>()->GetPlayerName();
			FString MyName = GetController()->GetPlayerState<APlayerState>()->GetPlayerName();
			GameState->SomeoneDeadEvent(AttackerName, MyName);
			GameState->UpdatePlayerScore(AttackerName, 10);
		}

		if (CameraShakeBase != nullptr && MyController != nullptr)
		{
			MyController->PlayHitAnimation();
			MyController->ClientStartCameraShake(CameraShakeBase, 1.f);
		}

		FVector ImpulseDirection = Attacker->GetActorForwardVector() * 10000.f;
		Dead_NMC(ImpulseDirection);
	}

	SetGhostMode();

	/*FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ThisClass::SetGhostMode, 3.f, false);*/
}

void AINHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ThisClass::SetSpeed, true);
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ThisClass::SetSpeed, false);
	}
}

void AINHCharacter::OnCollision()
{
	Super::OnCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AINHCharacter::OffCollision()
{
	Super::OffCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AINHCharacter::Action()
{
	Super::Action();

	if (StateComponent == nullptr || GetCharacterMovement()->IsFalling() || !StateComponent->IsIdleMode()) return;

	StateComponent->SetActionMode();

	if (HasAuthority())
		PlayAnimMontage_NMC(PunchMontage);

	else
		PlayAnimMontage_Server(PunchMontage);
}

void AINHCharacter::End_Action()
{
	Super::End_Action();
}

void AINHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, canRun);
	DOREPLIFETIME(ThisClass, CurrentSpeed);
}

void AINHCharacter::SetSpeed_Server_Implementation(bool InCanRun)
{
	canRun = InCanRun;
}

void AINHCharacter::SetSpeed(bool InIsRun)
{
	isRun = InIsRun;

	if (isRun)
		SetSpeed_Server(true);

	else
		SetSpeed_Server(false);
}

void AINHCharacter::PlayAnimMontage_Server_Implementation(UAnimMontage* InAnimMontage)
{
	PlayAnimMontage_NMC(InAnimMontage);
}

void AINHCharacter::PlayAnimMontage_NMC_Implementation(UAnimMontage* InAnimMontage)
{
	if(InAnimMontage != nullptr)
		PlayAnimMontage(InAnimMontage);
}

void AINHCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && HasAuthority())
	{
		IIDamage* HittedCharacter = Cast<IIDamage>(OtherActor);

		if (HittedCharacter != nullptr)
			HittedCharacter->Hit(this, FHitData());
	}
}

void AINHCharacter::DeadMaterialEvent_Implementation()
{
}

void AINHCharacter::Dead_NMC_Implementation(FVector InImpulseDirection)
{
	MoveComponent->CanMove = false;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	InImpulseDirection.Z = 3000.f;

	GetMesh()->AddImpulse(InImpulseDirection, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AINHCharacter::SetGhostMode()
{
	AINHGameMode* GameMode = Cast<AINHGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode)
		GameMode->SetGhostMode(GetController());
}