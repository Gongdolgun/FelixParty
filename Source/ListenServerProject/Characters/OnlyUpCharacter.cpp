#include "Characters/OnlyUpCharacter.h"
#include "Components/ParkourComponent.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

AOnlyUpCharacter::AOnlyUpCharacter()
{
	Helpers::CreateActorComponent<UParkourComponent>(this, &ParkourComponent, "Parkour");
	Helpers::CreateActorComponent<UStateComponent>(this, &StateComponent, "State");
	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("CMotionWarp"));

	// 액터 자체 리플리케이션
	//SetReplicates(true);

	ParkourComponent->SetIsReplicated(true);
	StateComponent->SetIsReplicated(true);
	MotionWarpComponent->SetIsReplicated(true);

	Helpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());
	Arrows.SetNum((int32)EParkourArrowType::Max);
	
	for (int32 i = 0; i < Arrows.Num(); i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		Helpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);
	
		EParkourArrowType ArrowType = static_cast<EParkourArrowType>(i);
	
		switch (ArrowType)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
			break;
	
		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, -30.0f, 20.0f));
			break;
	
		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0.0f, 30.0f, 20.0f));
			break;
		}
	}
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

void AOnlyUpCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Initial_Trace_Length);
	DOREPLIFETIME(ThisClass, Trace_Z_Offset);
	DOREPLIFETIME(ThisClass, Falling_Height_Multiplier);

	DOREPLIFETIME(ThisClass, ZOffset_Hand);
	DOREPLIFETIME(ThisClass, ZOffset_Landing);
	DOREPLIFETIME(ThisClass, Montage_Length);

}

void AOnlyUpCharacter::Action()
{
	Super::Action();

}

void AOnlyUpCharacter::Jump()
{
	Super::Jump();

}

void AOnlyUpCharacter::SetModeAndCollision_NMC_Implementation(EMovementMode InMovementMode, bool InCollision)
{
	// 파쿠르 시작하면 충돌체 끄면서 모드 변경
	SetActorEnableCollision(InCollision);
	GetCharacterMovement()->SetMovementMode(InMovementMode);
}

void AOnlyUpCharacter::SetModeAndCollision_Server_Implementation(EMovementMode InMovementMode, bool InCollision)
{
	SetModeAndCollision_NMC(InMovementMode, InCollision);
}

void AOnlyUpCharacter::SetModeAndCollision(EMovementMode InMovementMode, bool InCollision)
{
	if (IsLocallyControlled())
	{
		SetModeAndCollision_Server(InMovementMode, InCollision);
	}
}

void AOnlyUpCharacter::PlayParkour(FVector InParkourPos1, FVector InParkourPos2, float InZOffsetHand,
	float InZOffsetLanding, float InMontageLength)
{
	SetModeAndCollision(EMovementMode::MOVE_Flying, false);
	
	FVector ParkourPos1 = InParkourPos1 + FVector(0.0f, 0.0f, InZOffsetHand);
	FVector ParkourPos2 = InParkourPos2 + FVector(0.0f, 0.0f, (InZOffsetLanding - InZOffsetHand));
	
	FMotionWarpingTarget Target1;
	FName motion_Name1 = FName("ParkourPoint1");
	Target1.Name = motion_Name1;
	Target1.Location = ParkourPos1;
	Target1.Rotation = GetActorRotation();
	MotionWarpComponent->AddOrUpdateWarpTarget(Target1);
	
	FMotionWarpingTarget Target2;
	FName motion_Name2 = FName("ParkourPoint2");
	Target2.Name = motion_Name2;
	Target2.Location = ParkourPos2;
	Target2.Rotation = GetActorRotation();
	MotionWarpComponent->AddOrUpdateWarpTarget(Target2);
	
	PlayParkourMontage();
	
	FTimerHandle timerhandler;
	GetWorld()->GetTimerManager().SetTimer(timerhandler, FTimerDelegate::CreateLambda([this]() {
		SetModeAndCollision(EMovementMode::MOVE_Falling, true);
		}), InMontageLength, false);
}

void AOnlyUpCharacter::PlayParkourMontage_NMC_Implementation()
{
	PlayAnimMontage(ParkourComponent->ParkourMontage);
}

void AOnlyUpCharacter::PlayParkourMontage_Server_Implementation()
{
	PlayParkourMontage_NMC();
}

void AOnlyUpCharacter::PlayParkourMontage()
{
	if (IsLocallyControlled())
	{
		PlayParkourMontage_Server();
	}


}


