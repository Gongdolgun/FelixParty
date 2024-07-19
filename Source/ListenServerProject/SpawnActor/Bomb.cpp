#include "SpawnActor/Bomb.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	bBombReplicate = true;
	bBombReplicateMovement = true;

	Helpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	Helpers::CreateComponent<UAudioComponent>(this, &Audio, "Audio", Sphere);

	Sphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionObjectType(ECC_PhysicsBody);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ABomb::BeginPlay()
{
	Super::BeginPlay();

	if (CountdownSound)
	{
		Audio->SetSound(CountdownSound);
	}
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ElapseTime < TotalCountdownTime)
	{
		ElapseTime += DeltaTime;
		UpDateSound(DeltaTime);
	}
}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버에서 클라이언트로 값 전송
	DOREPLIFETIME(ABomb, bBombReplicate);
	DOREPLIFETIME(ABomb, bBombReplicateMovement);
	DOREPLIFETIME(ABomb, BombLocation);
	DOREPLIFETIME(ABomb, ElapseTime);

}

// 복제된 속성이 변경될 때 자동으로 호출
void ABomb::OnRep_UpdateBombLocation()
{
	// 클라이언트들에게 위치 변경을 알리기 위해
	SetActorLocation(BombLocation);
	CLog::Log(*BombLocation.ToString());
}

void ABomb::Explosion_Implementation()
{
	if (Particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector, true);
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		// 사운드 재생 후 0.2초 후에 폭탄을 파괴
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DestroyBomb, this, &ABomb::DestroyBomb, 0.2f, false);
	}
}

void ABomb::DestroyBomb()
{
	this->Destroy();
}

void ABomb::StartCountdown()
{
	ElapseTime = 0;

	MultiPlayCountdownSound();

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ABomb::Explosion, 10.0f, false);
}

void ABomb::MultiPlayCountdownSound_Implementation()
{
	if (CountdownSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CountdownSound, GetActorLocation());
	}
}

void ABomb::MultiPlayExplosionSound_Implementation()
{
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}

void ABomb::UpDateSound(float DeltaTime)
{
	float remainingTime = TotalCountdownTime - ElapseTime;

	if (DynamicMaterial)
	{
		float progress = 1.0f - (remainingTime / TotalCountdownTime);
		DynamicMaterial->SetScalarParameterValue(FName("Progress"), progress);
	}

	if (Audio && !Audio->IsPlaying())
	{
		Audio->Play();
	}

	float shakeintensity = FMath::Clamp(1.0f - (remainingTime / TotalCountdownTime), 0.0f, 1.0f);

	FVector newLocation = GetActorLocation();
	newLocation.X += FMath::Sin(GetWorld()->GetTimeSeconds() * 10.0f * shakeintensity) * 5.0f;
	newLocation.Y += FMath::Cos(GetWorld()->GetTimeSeconds() * 10.0f * shakeintensity) * 5.0f;
	SetActorLocation(newLocation);

}
