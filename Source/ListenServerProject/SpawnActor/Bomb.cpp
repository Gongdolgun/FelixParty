#include "SpawnActor/Bomb.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	Helpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Sphere);

}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

