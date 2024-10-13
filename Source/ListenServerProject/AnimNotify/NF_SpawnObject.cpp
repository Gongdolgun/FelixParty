#include "AnimNotify/NF_SpawnObject.h"
#include "Global.h"
#include "Characters/PushCharacter.h"
#include "GameFramework/Character.h"

FString UNF_SpawnObject::GetNotifyName_Implementation() const
{
	return "SpawnObject";
}

void UNF_SpawnObject::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp == nullptr) return;
	if (MeshComp->GetOwner() == nullptr) return;

	APushCharacter* OwnerCharacter = Cast<APushCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter == nullptr) return;
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (PlayerController == nullptr) return;

	if (ActorClass && OwnerCharacter->IsLocallyControlled())
	{
		// ȭ�� ũ�⸦ �����ͼ� �߾��� ��ũ�� ��ǥ ���
		FIntPoint Size = GEngine->GameViewport->Viewport->GetSizeXY();
		FVector2D ScreenCenter = FVector2D(Size.X, Size.Y) * 0.5f;

		// ��ũ�� ��ǥ�� ���� ��ǥ�� ��ȯ
		FVector WorldLocation;
		FVector WorldDirection;
		PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

		FTransform transform;
		transform.SetLocation(OwnerCharacter->GetActorLocation() +
			(OwnerCharacter->GetActorForwardVector() * 100.0f) + (OwnerCharacter->GetActorRightVector() * 15.0f));
		transform.SetRotation(FQuat(WorldDirection.Rotation()));

		OwnerCharacter->SpawnObject_Server(ActorClass, transform);
		
	}
}
