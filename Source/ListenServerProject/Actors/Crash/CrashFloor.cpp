#include "Actors/Crash/CrashFloor.h"
#include "Utilites/Helpers.h"

ACrashFloor::ACrashFloor()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Root);

	Count = 0;
}

void ACrashFloor::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMesh)
	{
		DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
	}
}

void ACrashFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrashFloor::LifeCount()
{
	Count++;

	ChangeColor();

	if (Count >= 3)
	{
		Destroy();
	}
}

void ACrashFloor::ChangeColor()
{
	if (DynamicMaterial)
	{
		if (Count == 0)
		{
			FloorColor = FLinearColor::Green;
		}
		else if (Count == 1)
		{
			FloorColor = FLinearColor::Yellow;
		}
		else if (Count == 2)
		{
			FloorColor = FLinearColor::Red;
		}

		DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), FloorColor);
	}
}

