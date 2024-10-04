#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NF_SpawnObject.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UNF_SpawnObject : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere)
	class UClass* ActorClass;

	UPROPERTY(EditAnywhere)
	float LeftDistance = -30.0f;




};
