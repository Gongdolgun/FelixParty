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
	SwingElapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ
	SwingAmount = 0.0f; // �ʱ� ��鸲 ����


	GetWorld()->GetTimerManager().SetTimer(SwingTimerHandle, this, &ThisClass::Swing, 0.1f, true);
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::DropObject, 2.0f, false);
}

void ADropObject::Swing()
{
	CLog::Print("Swing");
	// ��� �ð� ����
	SwingElapsedTime += 0.1f; // 0.1�ʸ��� ȣ��ǹǷ� ��� �ð��� 0.1�� ����

	// SwingAmount�� ���������� ������Ŵ
	SwingAmount = FMath::Lerp(0.0f, MaxSwingAmount, SwingElapsedTime / SwingDuration);

	// ���� ��ġ ���
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.X += FMath::Sin(GetWorld()->GetTimeSeconds() * 25.0f) * SwingAmount;
	CurrentLocation.Y += FMath::Cos(GetWorld()->GetTimeSeconds() * 25.0f) * SwingAmount;

	SetActorLocation(CurrentLocation);

	// SwingElapsedTime�� SwingDuration�� �����ϸ� Ÿ�̸� ����
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

	// 5�� �� ���� ��ġ�� ���ư��� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(DropTimerHandle, this, &ThisClass::RespawnOriginialLocation, 3.0f, false);
}

void ADropObject::RespawnOriginialLocation()
{
	CLog::Print("Call Me Respawn");

	GetWorld()->GetTimerManager().ClearTimer(DropTimerHandle);

	// ��ü ��Ȱ��ȭ
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// ���ο� ��ü ����
	FActorSpawnParameters SpawnParams; 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ���ο� ��ü�� ���� ��ġ�� ����
	ADropObject* NewDropObject = GetWorld()->SpawnActor<ADropObject>(GetClass(), OriginalLocation, FRotator::ZeroRotator, SpawnParams);

	// ���� ��ü ����
	Destroy();

}

