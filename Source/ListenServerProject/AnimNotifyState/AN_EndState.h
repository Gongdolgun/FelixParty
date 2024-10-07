#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_EndState.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UAN_EndState : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	ACharacter* OwnerCharacter;
<<<<<<< HEAD
};
=======
};
>>>>>>> parent of d6e129d (Merge remote-tracking branch 'origin/JH' into test)
