#include "SpawnActor/Bomb.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Controllers/DefaultController.h"
#include "GameModes/BombGameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh");
	Helpers::CreateComponent<UAudioComponent>(this, &Audio, "Audio", StaticMesh);
	Helpers::CreateComponent<UWidgetComponent>(this, &CountDownWidget, "CountDownWidget", StaticMesh);

	CountDownWidget->SetWidgetSpace(EWidgetSpace::Screen);
	ElapseTime = 0.0f;
	
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();

	CurrentExplosionTime = ExplosionTime;

	if (StaticMesh)
	{
		DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			OnRep_UpdateColor();  // 초기 색상 설정
		}
	}
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentExplosionTime > 0 && !bIsExplosion)
	{
		CurrentExplosionTime -= DeltaTime;
	}

	else
	{
		if(!bIsExplosion)
			Explosion();

		bIsExplosion = true;
	}

	if (ElapseTime < ExplosionTime)
	{
		ElapseTime += DeltaTime;
		UpDateSoundAndColor(DeltaTime);
	}
}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버에서 클라이언트로 값 전송
	DOREPLIFETIME(ThisClass, CurrentExplosionTime);
}

void ABomb::OnRep_CountdownSound()
{
	if (Audio && CountdownSound)
	{
		Audio->SetSound(CountdownSound);
		Audio->Play();
	}
}

void ABomb::OnRep_UpdateColor()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), BombColor);

		//UpdateShakeEffect(GetWorld()->GetDeltaSeconds());

	}
}

void ABomb::Explosion_Implementation()
{
	if (Particle != nullptr && ExplosionSound != nullptr && Owner != nullptr)
	{
		ExplosionEvent(GetActorLocation());

		Owner->DeadEvent_NMC();

		FTimerHandle RespawnTimer;

		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::CallRespawnbomb, 3.f, false);
	}
}

void ABomb::ExplosionEvent_Implementation(FVector InLocation)
{
	StaticMesh->SetHiddenInGame(true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, InLocation, FRotator::ZeroRotator, FVector::OneVector, true);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, InLocation);
}

void ABomb::MultiStartCountdown_Implementation()
{
	ElapseTime = 0.0f;

	if (Audio && CountdownSound)
	{
		Audio->SetSound(CountdownSound);
		Audio->Play();
	}

	//GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ABomb::Explosion, TotalCountdownTime, false);
}


void ABomb::CallRespawnbomb_Implementation()
{
	ABombGameMode* BombGameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode());
	ADefaultController* DeadController = Cast<ADefaultController>(Owner->GetController());

	if(BombGameMode != nullptr && DeadController != nullptr)
	{
		BombGameMode->SomeoneDead(DeadController);

		Destroy();
	}
}

void ABomb::UpDateSoundAndColor(float DeltaTime)
{
	float remainingTime = ExplosionTime - ElapseTime;

	float progress = ElapseTime / ExplosionTime;
	BombColor = FVector(FLinearColor::LerpUsingHSV(FLinearColor::Black, FLinearColor::Red, progress));

	if (HasAuthority())
	{
		OnRep_UpdateColor();  // 서버에서 직접 색상 업데이트 호출
	}
}

void ABomb::UpdateShakeEffect(float DeltaTime)
{
	float remainingTime = ExplosionTime - ElapseTime;
	float shakeIntensity = FMath::Clamp(1.0f - (remainingTime / ExplosionTime), 0.0f, 1.0f);
	FVector newLocation = GetActorLocation();
	newLocation.X += FMath::Sin(GetWorld()->GetTimeSeconds() * 20.0f * shakeIntensity) * 5.0f;
	newLocation.Y += FMath::Cos(GetWorld()->GetTimeSeconds() * 10.0f * shakeIntensity) * 5.0f;

	SetActorLocation(newLocation);
}

void ABomb::AttachBomb(ABombCharacter* InCharacter)
{
	if (Owner != nullptr)
	{
		Owner->bBomb = false;
		Owner->ChangeSpeed();
	}

	Owner = InCharacter;
	Owner->bBomb = true;
	Owner->ChangeSpeed();

	AttachToComponent(InCharacter->BombSphereComponent, FAttachmentTransformRules::KeepRelativeTransform);
}