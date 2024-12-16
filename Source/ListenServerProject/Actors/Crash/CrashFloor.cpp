#include "Actors/Crash/CrashFloor.h"
#include "Utilites/Helpers.h"
#include "DestructibleComponent.h"
#include "DestructibleMesh.h"

ACrashFloor::ACrashFloor()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");

	Count = 0;
}

void ACrashFloor::BeginPlay()
{
	Super::BeginPlay();

	
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
		//DestructibleMesh->ApplyRadiusDamage(100.0f, GetActorLocation(), 200.0f, 10.0f, true);
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
