#include "Components/ZoomComponent.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"

UZoomComponent::UZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = Helpers::GetComponent<USpringArmComponent>(GetOwner());

	CurrentValue = SpringArm->TargetArmLength;
}

void UZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, CurrentValue);
	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, CurrentValue, DeltaTime, InterpSpeed);

}

void UZoomComponent::SetZoomValue(float InValue)
{
	CurrentValue += (Speed * InValue);
	CurrentValue = FMath::Clamp(CurrentValue, Range.X, Range.Y);
}

