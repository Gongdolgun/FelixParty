#include "Components/ParkourComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"

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

void UParkourComponent::SetCanParkour(bool bInCanParkour)
{
	bCanParkour = bInCanParkour;
}

void UParkourComponent::ParkourTrace(float InInitialTraceLength, float InSecondaryTraceZOffset,
	float InFallingHeightMultiplier, TEnumAsByte<EDrawDebugTrace::Type> InDrawDebugType, FVector& InParkourPos1,
	FVector& InParkourPos2)
{
	InInitialTraceLength = Initial_Trace_Length;
	InSecondaryTraceZOffset = Trace_Z_Offset;
	InFallingHeightMultiplier = Falling_Height_Multiplier;

	// 초기 파쿠르 가능 값은 False일 것이다.
	SetCanParkour(false);

	FHitResult hitResult;
	FVector object_Start = OwnerCharacter->GetActorLocation();
	FVector object_End = object_Start + (OwnerCharacter->GetActorForwardVector() * Initial_Trace_Length);
	ECollisionChannel channel = ECollisionChannel::ECC_Visibility;
	TArray<AActor*> ignores;

	// 오브젝트 체크
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), object_Start, object_End, TraceType, false, ignores, DrawDebug_Parkour, hitResult, true);

	// 오브젝트가 발견되면 높이 추적 후 Player가 Falling 상태면 도달 가능 높이 낮추기 (파쿠르 자연스럽게 하기 위해)
	if (hitResult.bBlockingHit)
	{
		
	}

	else
	{
		// 오브젝트 충돌이 되지 않았다면 파쿠르가 되지 않도록
		SetCanParkour(false);
	}
}














