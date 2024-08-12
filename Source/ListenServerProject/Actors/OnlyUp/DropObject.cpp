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
	SwingElapsedTime = 0.0f; // 경과 시간 초기화
	SwingAmount = 0.0f; // 초기 흔들림 강도


	GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &ThisClass::Swing, 0.1f, true);
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::DropObject, 2.0f, false);
}

void ADropObject::Swing()
{
	CLog::Print("Swing");
	// 경과 시간 증가
	SwingElapsedTime += 0.1f; // 0.1초마다 호출되므로 경과 시간을 0.1초 증가

	// SwingAmount를 점진적으로 증가시킴
	SwingAmount = FMath::Lerp(0.0f, MaxSwingAmount, SwingElapsedTime / SwingDuration);

	// 현재 위치 계산
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.X += FMath::Sin(GetWorld()->GetTimeSeconds() * 25.0f) * SwingAmount;
	CurrentLocation.Y += FMath::Cos(GetWorld()->GetTimeSeconds() * 25.0f) * SwingAmount;

	SetActorLocation(CurrentLocation);

	// SwingElapsedTime가 SwingDuration에 도달하면 타이머 정리
	if (SwingElapsedTime >= SwingDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(SwingTimerHandle);
	}
}

void ADropObject::DropObject()
{
	CLog::Print("Drop");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->AddImpulse(FVector(0, 0, -500.0f), NAME_None, true);

	// 5초 후 원래 위치로 돌아가는 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::RespawnOriginialLocation, 3.0f, false);
}

void ADropObject::RespawnOriginialLocation()
{
	CLog::Print("Call Me Respawn");

	GetWorld()->GetTimerManager().ClearTimer(DropTimerHandle);

	// 객체 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 새로운 객체 스폰
	FActorSpawnParameters SpawnParams; 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 새로운 객체를 원래 위치에 스폰
	ADropObject* NewDropObject = GetWorld()->SpawnActor<ADropObject>(GetClass(), OriginalLocation, FRotator::ZeroRotator, SpawnParams);

	// 기존 객체 삭제
	Destroy();

}

