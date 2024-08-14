#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NFS_Movement.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UNFS_Movement : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
