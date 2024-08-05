#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyPlatforms.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyPlatforms : public AActor
{
	GENERATED_BODY()
	
public:	
	ALobbyPlatforms();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SpawnCharacter(class ALobbyController* InLobbyController);
	void ClearCharacter();

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Cube;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* PlayerSpawnTransform;

private:
	UPROPERTY(EditAnywhere, Category = "LobbyClass")
	TSubclassOf<class ALobbyCharacter> LobbyCharacter_Class;

public:
	ALobbyController* LobbyController;
	AActor* CurrentCharacter;

};
