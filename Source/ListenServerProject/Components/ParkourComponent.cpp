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

void UParkourComponent::ParkourTrace(float InitialTraceLength, float SecondaryTraceZOffset,
	float FallingHeightMultiplier, FVector& InParkourPos1, FVector& InParkourPos2)
{

}









