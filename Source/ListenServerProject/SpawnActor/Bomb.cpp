#include "SpawnActor/Bomb.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bBombReplicate = true;
	bBombReplicateMovement = true;

	Helpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	Helpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Niagara", Sphere);

	Sphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionObjectType(ECC_PhysicsBody);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버에서 클라이언트로 값 전송
	DOREPLIFETIME(ABomb, bBombReplicate);
	DOREPLIFETIME(ABomb, bBombReplicateMovement);
	DOREPLIFETIME(ABomb, BombLocation);

}

// 복제된 속성이 변경될 때 자동으로 호출
void ABomb::OnRep_UpdateBombLocation()
{
	// 클라이언트들에게 위치 변경을 알리기 위해
	SetActorLocation(BombLocation);
	CLog::Log(*BombLocation.ToString());
}

