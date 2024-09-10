#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "INHMain.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UINHMain : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayHitAnimation();
	void PlayHitAnimation_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SomeoneDead(const FString& InAttackerName, const FString& InHittedCharacterName);
	void SomeoneDead_Implementation(const FString& InAttackerName, const FString& InHittedCharacterName);
};