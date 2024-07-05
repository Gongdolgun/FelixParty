#include "AnimNotifyState/NFS_Equip.h"
#include "Global.h"
#include "Characters/DefaultCharacter.h"
#include "Components/WeaponComponent.h"

FString UNFS_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UNFS_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp && MeshComp->GetOwner())
	{
		UWeaponComponent* weapon = Helpers::GetComponent<UWeaponComponent>(MeshComp->GetOwner());
		ADefaultCharacter* Player = Cast<ADefaultCharacter>(MeshComp->GetOwner());

		if (weapon && Player)
		{
			weapon->Begin_Equip();
		}

	}
}

void UNFS_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner())
	{
		UWeaponComponent* weapon = Helpers::GetComponent<UWeaponComponent>(MeshComp->GetOwner());

		if (weapon)
		{
			weapon->End_Equip();
		}
	}

}

