#include "Characters/OnlyUpCharacter.h"
#include "Components/ParkourComponent.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AOnlyUpCharacter::AOnlyUpCharacter()
{
	Helpers::CreateActorComponent<UParkourComponent>(this, &ParkourComponent, "ParkourComponent");
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComponent"));

	SpringArm->SocketOffset = FVector(60.0f, 0.0f, 0.0f);
}

void AOnlyUpCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AOnlyUpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOnlyUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AOnlyUpCharacter::Action()
{
	Super::Action();

}

void AOnlyUpCharacter::Jump()
{
	Super::Jump();
	// ExecuteParkour(ZOffset_Hand, ZOffset_Landing, Montage_Length);
}

void AOnlyUpCharacter::ExecuteParkour(float InZOffset_Hand, float InZOffset_Landing, float InMontageLength)
{
	ParkourComponent->ParkourTrace(
		InZOffset_Hand,
		InZOffset_Landing,
		InMontageLength);

	// 파쿠르가 가능하면
	if (ParkourComponent->GetCanParkour() == true)
	{
		// 파쿠르 시작하면 충돌체 끄기.
		SetActorEnableCollision(false);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		FMotionWarpingTarget Target1 = {};
		FName motion_Name1 = FName("ParkourPoint1");
		Target1.Name = motion_Name1;
		Target1.Location = ParkourComponent->GetParkourPos1() + FVector(0.0f, 0.0f, InZOffset_Hand);
		Target1.Rotation = GetActorRotation();
		MotionWarpComponent->AddOrUpdateWarpTarget(Target1);

		FMotionWarpingTarget Target2 = {};
		FName motion_Name2 = FName("ParkourPoint2");
		Target2.Name = motion_Name2;
		Target2.Location = ParkourComponent->GetParkourPos2() + FVector(0.0f, 0.0f, InZOffset_Landing);
		Target2.Rotation = GetActorRotation();
		MotionWarpComponent->AddOrUpdateWarpTarget(Target2);

		PlayAnimMontage(ParkourComponent->ParkourMontage);

		FTimerHandle timerhandler;
		GetWorld()->GetTimerManager().SetTimer(timerhandler, FTimerDelegate::CreateLambda([this]() {
			SetActorEnableCollision(true);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			}), InMontageLength, false);
	}

	else
	{
		ACharacter::Jump();
	}
}


