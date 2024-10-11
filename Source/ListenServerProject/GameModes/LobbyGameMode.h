#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/Structures.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerLists();

public:
	// �κ� ���Ӹ�忡�� ������ �÷��̾� ��Ʈ�ѷ���� �⺻ ������
	UPROPERTY(BlueprintReadOnly)
	TArray<class ALobbyController*> ConnectedPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerBaseInfo> PlayerBaseInfos;

	// �⺻ �÷��̾� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerData DefaultPlayerData;

public:
	void UpdatePlayerMaterial();
	void LeaveSession(ALobbyController* InController);
};