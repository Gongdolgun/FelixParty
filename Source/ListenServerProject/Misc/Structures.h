#pragma once

#include "CoreMinimal.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FPlayerBaseInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerName = FName(TEXT("Player Name"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* UserThumbnail;
};

USTRUCT(BlueprintType)
struct FPlayerInGameData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADefaultCharacter> CharacterClass;
};
