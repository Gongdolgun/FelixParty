#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NF_SpawnActor.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UNF_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> ActorClass;

	UPROPERTY(EditAnywhere)
	float Rate;

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
