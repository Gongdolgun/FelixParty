#include "GameModes/BombGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Engine/TargetPoint.h"
#include "Global.h"

ABombGameMode::ABombGameMode()
{

}

void ABombGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> groundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), groundActors);
	for (AActor* Point : groundActors)
	{
		if (Point->IsA(ATargetPoint::StaticClass()))
		{
			SpawnPoints.Add(Point);
		}
	}

	SpawnCharacters();
}

void ABombGameMode::SpawnCharacters()
{
	if (CharacterClass == nullptr)
		return;


	if (SpawnPoints.Num() < NumberCharacters)
		return;

	// ���� ���� ���� ����
	for (int32 i = 0; i < SpawnPoints.Num(); i++)
	{
		int32 swapIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		SpawnPoints.Swap(i, swapIndex);
	}

	// ĳ���� ���� �� ���� ���� ���� ��ġ
	for (int32 i = 0; i < NumberCharacters; i++)
	{
		FActorSpawnParameters spawnParams;
		GetWorld()->SpawnActor<ACharacter>(CharacterClass, SpawnPoints[i].Get()->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	}

}
