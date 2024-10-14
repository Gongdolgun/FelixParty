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

	if (CurrentExplosionTime > 0 && !bIsExplosion)
	{
		CurrentExplosionTime -= DeltaTime;
	}

	else
	{
		if (!bIsExplosion)
			Explosion();

		bIsExplosion = true;
	}

	if (ElapseTime < ExplosionTime)
	{
		ElapseTime += DeltaTime;
		UpDateColor(DeltaTime);
	}

	UpdateShakeEffect(DeltaTime);

}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버에서 클라이언트로 값 전송
	DOREPLIFETIME(ThisClass, CurrentExplosionTime);
}

void ABomb::OnRep_UpdateColor()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), BombColor);
	}
}

void ABomb::Explosion_Implementation()
{
	if (Particle != nullptr && ExplosionSound != nullptr && Owner != nullptr)
	{
		ExplosionEvent(GetActorLocation());

		Owner->DeadEvent_NMC();

		if (CountDownWidget)
		{
			CountDownWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden); 
		}

		FTimerHandle RespawnTimer;

		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::CallRespawnbomb, 3.0f, false);
	}
}

void ABomb::ExplosionEvent_Implementation(FVector InLocation)
{
	StaticMesh->SetHiddenInGame(true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, InLocation, FRotator::ZeroRotator, FVector::OneVector, true);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, InLocation);
	Audio->Stop();
}

void ABomb::CallRespawnbomb_Implementation()
{
	ABombGameMode* BombGameMode = Cast<ABombGameMode>(GetWorld()->GetAuthGameMode());
	ADefaultController* DeadController = Cast<ADefaultController>(Owner->GetController());

	if (BombGameMode != nullptr && DeadController != nullptr)
	{
		BombGameMode->SomeoneDead(DeadController);

		if (CountDownWidget)
		{
			CountDownWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		}

		Destroy();
	}
}

void ABomb::UpDateColor(float DeltaTime)
{
	float remainingTime = ExplosionTime - ElapseTime;

	float progress = ElapseTime / ExplosionTime;
	BombColor = FVector(FLinearColor::LerpUsingHSV(FLinearColor::Black, FLinearColor::Red, progress));

	OnRep_UpdateColor(); // 서버에서 직접 색상 업데이트 호출
	
}

void ABomb::UpdateShakeEffect(float DeltaTime)
{
	if (Owner && Owner->BombSphereComponent)
	{
		FVector sphereLocation = Owner->BombSphereComponent->GetComponentLocation();

		float shakeIntensity = FMath::Clamp(FMath::Sin(GetWorld()->GetTimeSeconds() * 2.0f), 3.0f, 5.0f);

		FVector shakeOffset;
		shakeOffset.X = FMath::Sin(GetWorld()->GetTimeSeconds() * 25.0f) * shakeIntensity * 5.0f; 
		shakeOffset.Y = FMath::Cos(GetWorld()->GetTimeSeconds() * 15.0f) * shakeIntensity * 5.0f;

		SetActorLocation(sphereLocation + shakeOffset);
	}
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