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
	// �ʱ� ���� ���� ���� False�� ���̴�.
	SetCanParkour(false);

	FHitResult object_HitResult;
	FVector object_Start = OwnerCharacter->GetActorLocation();
	FVector object_End = object_Start + (OwnerCharacter->GetActorForwardVector() * InInitialTraceLength);
	TArray<AActor*> object_ignores;

	// ������Ʈ üũ
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), object_Start, object_End, TraceType, false,
		object_ignores, DrawDebug_Parkour, object_HitResult, true);

	// ������Ʈ�� �߰ߵǸ� ���� ���� �� Player�� Falling ���¸� ���� ���� ���� ���߱� (���� �ڿ������� �ϱ� ����)
	if (object_HitResult.bBlockingHit)
	{
		bool falling = OwnerCharacter->GetCharacterMovement()->IsFalling();

		// �ٴڿ� ���� �� Height �� ����
		if (falling == false)
		{
			InFallingHeightMultiplier = 1.0f;
		}

		// ������
		float Correction_Height = InSecondaryTraceZOffset * InFallingHeightMultiplier;
		FVector falling_Start = object_HitResult.Location + FVector(0.0f, 0.0f, Correction_Height + Correction_Height_Relative);
		FVector falling_End = object_HitResult.Location;

		FHitResult falling_HitResult;
		TArray<AActor*> falling_ignores;

		UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), falling_Start, falling_End, 10.0f, TraceType, false, falling_ignores, DrawDebug_Parkour, falling_HitResult, true, FLinearColor::Blue, FLinearColor::Black);

		// �������� ����� ���� ����Ͽ� �̵� ������ ��ġ ã��
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
				// ������ġ
				ParkourPos2 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 50.0f);
			}

			// ù ��°�� �� ��° ���� ��ġ�� ��ΰ� ��Ȯ���� Ȯ���ϱ� ���� ������ Trace
			bool bCheck = false;
			bCheck |= UKismetMathLibrary::EqualEqual_VectorVector(ParkourPos1, FVector::ZeroVector);
			bCheck |= UKismetMathLibrary::EqualEqual_VectorVector(ParkourPos2, FVector::ZeroVector);

			if (bCheck)
			{
				SetCanParkour(false);
			}

			else
			{
				// last_TraceAdd�� �⺻���� 100����
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
		// ������Ʈ �浹�� ���� �ʾҴٸ� ������ ���� �ʵ���
		SetCanParkour(false);
	}
}



