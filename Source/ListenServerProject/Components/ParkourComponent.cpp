#include "Components/ParkourComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

UParkourComponent::UParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FVector UParkourComponent::GetParkourPos1()
{
	return ParkourPos1;
}

FVector UParkourComponent::GetParkourPos2()
{
	return ParkourPos2;
}

void UParkourComponent::SetCanParkour(bool bInCanParkour)
{
	bCanParkour = bInCanParkour;
}

void UParkourComponent::ParkourTrace(float InInitialTraceLength, float InSecondaryTraceZOffset, float InFallingHeightMultiplier)
{
	// 초기 파쿠르 가능 값은 False일 것이다.
	SetCanParkour(false);

	FHitResult object_HitResult;
	FVector object_Start = OwnerCharacter->GetActorLocation();
	FVector object_End = object_Start + (OwnerCharacter->GetActorForwardVector() * InInitialTraceLength);
	TArray<AActor*> object_ignores;

	// 오브젝트 체크
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), object_Start, object_End, TraceType, false,
		object_ignores, DrawDebug_Parkour, object_HitResult, true);

	// 오브젝트가 발견되면 높이 추적 후 Player가 Falling 상태면 도달 가능 높이 낮추기 (파쿠르 자연스럽게 하기 위해)
	if (object_HitResult.bBlockingHit)
	{
		bool falling = OwnerCharacter->GetCharacterMovement()->IsFalling();

		// 바닥에 있을 때 Height 값 조절
		if (falling == false)
		{
			InFallingHeightMultiplier = 1.0f;
		}

		// 보정값
		float Correction_Height = InSecondaryTraceZOffset * InFallingHeightMultiplier;
		FVector falling_Start = object_HitResult.Location + FVector(0.0f, 0.0f, Correction_Height + Correction_Height_Relative);
		FVector falling_End = object_HitResult.Location;

		FHitResult falling_HitResult;
		TArray<AActor*> falling_ignores;

		UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), falling_Start, falling_End, 10.0f, TraceType, false, falling_ignores, DrawDebug_Parkour, falling_HitResult, true, FLinearColor::Blue, FLinearColor::Black);

		// 추적에서 검출된 점을 사용하여 이동 워프할 위치 찾기
		if (falling_HitResult.bBlockingHit)
		{
			ParkourPos1 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * -50.0f);
			ParkourPos2 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 120.0f);

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
				// 워프위치
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

		}
	}

	else
	{
		// 오브젝트 충돌이 되지 않았다면 파쿠르가 되지 않도록
		SetCanParkour(false);
	}
}



