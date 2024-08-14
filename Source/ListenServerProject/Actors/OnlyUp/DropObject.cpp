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
}

void ADropObject::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();

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
	GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &ThisClass::Swing, 0.03f, true);
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::DropObject, 2.0f, false);
}

void ADropObject::Swing()
{
	CLog::Print("Swing");

	// 현재 Roll 각도 계산
	float CurrentRoll = FMath::Sin(GetWorld()->GetTimeSeconds() * SwingSpeed) * MaxSwingAmount;

	// Roll 적용
	FRotator NewRotation = FRotator(0.0f, 0.0f, CurrentRoll); // Pitch와 Yaw는 0으로 설정
	SetActorRotation(NewRotation);
}

void ADropObject::DropObject()
{
	CLog::Print("Drop");
	GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->AddImpulse(FVector(0, 0, -500.0f), NAME_None, true);

	// 5초 후 원래 위치로 돌아가는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::RespawnOriginialLocation, 3.0f, false);
}

void ADropObject::RespawnOriginialLocation()
{
	CLog::Print("Call Me Respawn");

	GetWorld()->GetTimerManager().ClearTimer(DropTimerHandle);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	FActorSpawnParameters SpawnParams; 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADropObject* NewObject = GetWorld()->SpawnActor<ADropObject>(GetClass(), OriginalLocation, FRotator::ZeroRotator, SpawnParams);

	Destroy();

}

