#include "Actors/OnlyUp/DropObject.h"
#include "Components/StaticMeshComponent.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ADropObject::ADropObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Root);

	Helpers::CreateComponent<UBoxComponent>(this, &Collision, "Collision", StaticMesh);

	StaticMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ADropObject::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
	OriginalRotation = GetActorRotation();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnComponentBeginOverlap);
}

void ADropObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ADropObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character && (OtherActor != this))
	{
		GetWorld()->GetTimerManager().SetTimer(
			SwingTimerHandle, this, &ThisClass::StartSwing, 3.0f, false);

	}
}

void ADropObject::StartSwing()
{
	StaticMesh->SetCollisionProfileName(TEXT("Object"));

	GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &ThisClass::Swing, 0.03f, true);
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::DropObject, 2.0f, false);
}

void ADropObject::Swing()
{
	// 현재 Roll 각도 계산
	float CurrentRoll = FMath::Sin(GetWorld()->GetTimeSeconds() * SwingSpeed) * MaxSwingAmount;

	FRotator NewRotation = FRotator(CurrentRoll, 0.0f, CurrentRoll);
	SetActorRotation(NewRotation);
}

void ADropObject::DropObject()
{
	GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->AddImpulse(FVector(0, 0, -1000.0f), NAME_None, true);

	// 5초 후 원래 위치로 돌아가는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::RespawnOriginialLocation, 3.0f, false);
}

void ADropObject::RespawnOriginialLocation()
{
	GetWorld()->GetTimerManager().ClearTimer(DropTimerHandle);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	FActorSpawnParameters SpawnParams; 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADropObject* NewObject = GetWorld()->SpawnActor<ADropObject>(GetClass(), OriginalLocation, OriginalRotation, SpawnParams);

	Destroy();
}

