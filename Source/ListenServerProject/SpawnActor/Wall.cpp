#include "SpawnActor/Wall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "SpawnActor/Wall.h"

AWall::AWall()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateComponent<UBoxComponent>(this, &Box, "Box");
	Helpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Box);
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Box);


}

void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

