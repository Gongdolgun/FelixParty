#include "Components/ChatComponent.h"

UChatComponent::UChatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UChatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UChatComponent::SendMessage_Server_Implementation(const FString& InMessage)
{
	SendMessage_NMC(InMessage);
}

void UChatComponent::SendMessage_NMC_Implementation(const FString& InMessage)
{
	CurrentMessage = InMessage;
}