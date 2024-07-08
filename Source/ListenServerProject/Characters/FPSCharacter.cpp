#include "Characters/FPSCharacter.h"
#include "AnimInstance_DefaultCharacter.h"

AFPSCharacter::AFPSCharacter()
{
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if(SkeletalMesh)
	{
		SkeletalMesh->SetAnimInstanceClass(AnimInstance);
		UAnimInstance_DefaultCharacter* FPSAnimInstance = Cast<UAnimInstance_DefaultCharacter>(SkeletalMesh->GetAnimInstance());

		if(FPSAnimInstance)
			FPSAnimInstance->OwnerCharacter = this;
	}
}
