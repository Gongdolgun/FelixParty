#include "Components/ParkourComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Components/ArrowComponent.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Arrows.SetNum((int32)EParkourArrowType::Max);
	HitResults.SetNum((int32)EParkourArrowType::Max);

}

void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	USceneComponent* arrow = Helpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");

	TArray<USceneComponent*> components;
	arrow->GetChildrenComponents(false, components);

	for (int32 i = 0; i < Arrows.Num(); i++)
	{
		Arrows[i] = Cast<UArrowComponent>(components[i]);
	}
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParkourComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, OwnerCharacter);

	DOREPLIFETIME(ThisClass, bCanParkour);
	DOREPLIFETIME(ThisClass, Correction_Height_Relative);
	DOREPLIFETIME(ThisClass, last_TraceAdd1);
	DOREPLIFETIME(ThisClass, last_TraceAdd2);

	DOREPLIFETIME(ThisClass, ParkourPos1);
	DOREPLIFETIME(ThisClass, ParkourPos1);

	DOREPLIFETIME(ThisClass, AddPlayerLocationForward);
	DOREPLIFETIME(ThisClass, AddPlayerLocationZ);

	DOREPLIFETIME(ThisClass, falling_ImpactPoint);
}

FVector UParkourComponent::GetParkourPos1()
{
	return ParkourPos1;
}

FVector UParkourComponent::GetParkourPos2()
{
	return ParkourPos2;
}

void UParkourComponent::CorrectPlayerLocation()
{
	FVector location = OwnerCharacter->GetActorLocation();
	FVector forward = OwnerCharacter->GetActorForwardVector();

	falling_ImpactPoint = FVector(location.X, location.Y,
		falling_ImpactPoint.Z + AddPlayerLocationZ) + FVector(forward * AddPlayerLocationForward);

	OwnerCharacter->SetActorLocation(falling_ImpactPoint);
}

void UParkourComponent::SetCanParkour(bool bInCanParkour)
{
	bCanParkour = bInCanParkour;
}

void UParkourComponent::ParkourTrace(FVector& OutLocation1, FVector& OutLocation2,
	float InInitialTraceLength, float InSecondaryTraceZOffset, float InFallingHeightMultiplier)
{
	if (OwnerCharacter == nullptr) return;
	// 초기 파쿠르 가능 값은 False일 것이다.
	SetCanParkour(false);

	ParkourPos1 = FVector::ZeroVector;
	ParkourPos2 = FVector::ZeroVector;

	LineTrace(EParkourArrowType::Center, InInitialTraceLength);
	LineTrace(EParkourArrowType::Left, InInitialTraceLength);
	LineTrace(EParkourArrowType::Right, InInitialTraceLength);

	bool bCheckTrace = true;
	for (int32 i = 0; i < Arrows.Num(); i++)
	{
		bCheckTrace &= HitResults[i].bBlockingHit;
	}

	// 3개의 Trace가 모두 충돌이 되었을 때
	if (bCheckTrace && Check_ObjectRotation())
	{
		bool falling = OwnerCharacter->GetCharacterMovement()->IsFalling();

		// 바닥에 있을 때 Height 값 조절
		if (falling == false)
		{
			InFallingHeightMultiplier = 1.0f;
		}

		// 보정값
		float Correction_Height = InSecondaryTraceZOffset * InFallingHeightMultiplier;
		FVector falling_Start = HitResults[(int32)EParkourArrowType::Center].Location + FVector(0.0f, 0.0f, Correction_Height + Correction_Height_Relative);
		FVector falling_End = HitResults[(int32)EParkourArrowType::Center].Location;

		FHitResult falling_HitResult;
		TArray<AActor*> falling_ignores;

		UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), falling_Start, falling_End, 10.0f, TraceType, false, falling_ignores, DrawDebug_Parkour, falling_HitResult, true,
			FLinearColor::Blue, FLinearColor::Black);

		// 추적에서 검출된 점을 사용하여 이동 워프할 위치 찾기
		if (falling_HitResult.bBlockingHit)
		{
			ParkourPos1 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * -50.0f);
			ParkourPos2 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 120.0f);

			falling_ImpactPoint = falling_HitResult.ImpactPoint;

			SetCanParkour(true);

			FVector parkour_Start = FVector(ParkourPos2.X, ParkourPos2.Y, ParkourPos2.Z + 20.0f);
			FVector parkour_End = FVector(ParkourPos2.X, ParkourPos2.Y, ParkourPos2.Z + 20.0f);

			FHitResult parkour_HitResult;
			TArray<AActor*> parkour_ignores;

			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), parkour_Start, parkour_End, 10.0f, TraceType, false, parkour_ignores, DrawDebug_Parkour, parkour_HitResult, true);

			if (parkour_HitResult.bBlockingHit)
			{
				SetCanParkour(false);
			}

			else
			{
				// 워프위치 50으로 고정
				ParkourPos2 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 50.0f);
			}

			// 첫 번째와 두 번째 파쿠르 위치의 경로가 명확한지 확인하기 위한 마지막 Trace
			bool bCheck = false;
			bCheck |= UKismetMathLibrary::EqualEqual_VectorVector(ParkourPos1, FVector::ZeroVector);
			bCheck |= UKismetMathLibrary::EqualEqual_VectorVector(ParkourPos2, FVector::ZeroVector);

			if (bCheck)
			{
				SetCanParkour(false);
			}

			else
			{
				// last_TraceAdd의 기본값은 100으로
				FVector last_Start = FVector(ParkourPos1.X, ParkourPos1.Y, ParkourPos2.Z + last_TraceAdd1);
				FVector last_End = FVector(ParkourPos2.X, ParkourPos2.Y, ParkourPos2.Z + last_TraceAdd2);

				FHitResult last_HitResult;
				TArray<AActor*> last_ignores;

				UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), last_Start, last_End, 10.0f, TraceType, false, last_ignores, DrawDebug_Parkour, last_HitResult, true);

				if (last_HitResult.bBlockingHit)
				{
					SetCanParkour(false);
				}

			}

			OutLocation1 = ParkourPos1;
			OutLocation2 = ParkourPos2;
		}

	}

	else
	{
		// 오브젝트 충돌이 되지 않았다면 파쿠르가 되지 않도록
		SetCanParkour(false);
	}
}

void UParkourComponent::LineTrace(EParkourArrowType InType, float InInitialTraceLength)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + (OwnerCharacter->GetActorForwardVector() * InInitialTraceLength);
	TArray<AActor*> ignores;

	// 오브젝트 체크
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), start, end, TraceType, false,
		ignores, DrawDebug_Parkour, HitResults[(int32)InType], true, color, FLinearColor::White);


}

bool UParkourComponent::Check_ObjectRotation()
{
	FVector center = HitResults[(int32)EParkourArrowType::Center].Normal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].Normal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].Normal;

	if (center.Equals(left) == false) return false;
	if (center.Equals(right) == false) return false;

	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();
	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;

	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

	float yaw = abs(abs(lookAt) - abs(impactAt));

	if (yaw >= AvailableFrontAngle) return false;

	return true;

}
