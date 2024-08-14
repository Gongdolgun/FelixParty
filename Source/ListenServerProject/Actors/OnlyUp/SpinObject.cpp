#include "Actors/OnlyUp/SpinObject.h"
#include "Components/StaticMeshComponent.h"
#include "Global.h"

ASpinObject::ASpinObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh1, "StaticMesh1", Root);
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh2, "StaticMesh2", Root);
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh3, "StaticMesh3", Root);

	RotationSpeed = FRotator(45.0f, 0.0f, 0.0f);
}

void ASpinObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpinObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator RotationDelta = RotationSpeed * DeltaTime;
	AddActorLocalRotation(RotationDelta);
}

