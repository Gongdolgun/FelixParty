// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitBlood.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API UHitBlood : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayHitAnimation();
	void PlayHitAnimation_Implementation();
	
};
