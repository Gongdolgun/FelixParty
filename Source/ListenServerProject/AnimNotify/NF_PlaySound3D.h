#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NF_PlaySound3D.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UNF_PlaySound3D : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundWave* Sound;

private:
	ACharacter* Owner;
};
