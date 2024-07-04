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

		weapon->Begin_Equip(WeaponIndex);

		/*ADefaultCharacter* Player = Cast<ADefaultCharacter>(MeshComp->GetOwner());

		if (Player)
		{
			Player->Weapon->Begin_Equip(WeaponIndex);
		}*/
	}
	printf("false");

	//weapon->Begin_Equip();
}

void UNFS_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	//if (!MeshComp && !MeshComp->GetOwner())
	//	return;

	//UWeaponComponent* weapon = Helpers::GetComponent<UWeaponComponent>(MeshComp->GetOwner());

	//weapon->End_Equip();

}

