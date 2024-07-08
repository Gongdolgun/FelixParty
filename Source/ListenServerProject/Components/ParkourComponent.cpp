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

	// �ʱ� ���� ���� ���� False�� ���̴�.
	SetCanParkour(false);

	FHitResult hitResult;
	FVector object_Start = OwnerCharacter->GetActorLocation();
	FVector object_End = object_Start + (OwnerCharacter->GetActorForwardVector() * Initial_Trace_Length);
	ECollisionChannel channel = ECollisionChannel::ECC_Visibility;
	TArray<AActor*> ignores;

	// ������Ʈ üũ
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), object_Start, object_End, TraceType, false, ignores, DrawDebug_Parkour, hitResult, true);

	// ������Ʈ�� �߰ߵǸ� ���� ���� �� Player�� Falling ���¸� ���� ���� ���� ���߱� (���� �ڿ������� �ϱ� ����)
	if (hitResult.bBlockingHit)
	{
		
	}

	else
	{
		// ������Ʈ �浹�� ���� �ʾҴٸ� ������ ���� �ʵ���
		SetCanParkour(false);
	}
}














