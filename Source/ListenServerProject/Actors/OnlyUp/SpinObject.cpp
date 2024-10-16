#include "Actors/OnlyUp/SpinObject.h"
#include "Components/StaticMeshComponent.h"
#include "Global.h"
#include "GameState/DefaultGameState.h"
#include "Net/UnrealNetwork.h"

ASpinObject::ASpinObject()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh1, "StaticMesh1", Root);
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh2, "StaticMesh2", Root);
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh3, "StaticMesh3", Root);

}

void ASpinObject::BeginPlay()
{
	Super::BeginPlay();

	ADefaultGameState* gameState = Cast<ADefaultGameState>(UGameplayStatics::GetGameState(this));
	if (gameState)
	{
		gameState->OnGameStateTypeChanged.AddDynamic(this, &ASpinObject::OnGamePlayStart);
	}

	switch (RotationType)
	{
	case EActorRotationType::Pitch:
		RotationSpeed = FRotator(RotationSpeed.Pitch, 0.0f, 0.0f);
		break;

	case EActorRotationType::Yaw:
		RotationSpeed = FRotator(0.0f, RotationSpeed.Yaw, 0.0f);
		break;

	case EActorRotationType::Roll:
		RotationSpeed = FRotator(0.0f, 0.0f, RotationSpeed.Roll);
		break;
	}
}

void ASpinObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RotationDelta);
	DOREPLIFETIME(ThisClass, RotationSpeed);
	DOREPLIFETIME(ThisClass, bCheck);
}

void ASpinObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		RotationDelta = RotationSpeed * DeltaTime;
		AddRotation_NMC(RotationDelta);

	}
}

void ASpinObject::AddRotation_NMC_Implementation(FRotator InRotationDelta)
{
	FRotator tempRotation = FRotator(InRotationDelta);
	AddActorLocalRotation(tempRotation);
}

void ASpinObject::OnGamePlayStart(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if (InNewGameType == EGameStateType::GameStart)
	{
		bCheck = true;
	}
}

