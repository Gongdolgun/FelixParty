#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LISTENSERVERPROJECT_API UChatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UChatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintReadWrite)
	FString CurrentMessage;

public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SendMessage_Server(const FString& InMessage);

	UFUNCTION(NetMulticast, Reliable)
	void SendMessage_NMC(const FString& InMessage);
	
};
