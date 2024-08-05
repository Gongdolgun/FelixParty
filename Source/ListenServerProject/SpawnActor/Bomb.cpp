#include "SpawnActor/Bomb.h"
#include "Global.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
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
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Sphere);
	Helpers::CreateComponent<UWidgetComponent>(this, &CountDownWidget, "CountDownWidget", Sphere);

	Sphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionObjectType(ECC_PhysicsBody);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CountDownWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TotalCountdownTime = 10.0f;
	ElapseTime = 0.0f;
	
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMesh)
	{
		DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			OnRep_UpdateColor();  // �ʱ� ���� ����
		}
	}

	if (HasAuthority())
	{
		StartCountdown();
	}

	if (CountdownWidgetClass)
	{
		if (CountDownWidget)
		{
			CountDownWidget->SetWidgetClass(CountdownWidgetClass);
			CountDownWidget->InitWidget();

			UCountDown* CountdownWidget = Cast<UCountDown>(CountDownWidget->GetWidget());
			if (CountdownWidget)
			{
				CountdownWidget->UpdateCountdown(10); // �ʱ� ī��Ʈ�ٿ� ���� ����
			}

			CountDownWidget->AttachToComponent(Sphere, FAttachmentTransformRules::KeepRelativeTransform);
			CountDownWidget->SetDrawAtDesiredSize(true);
		}
	}
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ElapseTime < TotalCountdownTime)
	{
		ElapseTime += DeltaTime;
		UpDateSoundAndColor(DeltaTime);
		UpdateShakeEffect(DeltaTime);
		UpdateCountdownWidget(DeltaTime);
	}
}

void ABomb::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �������� Ŭ���̾�Ʈ�� �� ����
	DOREPLIFETIME(ABomb, bBombReplicate);
	DOREPLIFETIME(ABomb, bBombReplicateMovement);
	DOREPLIFETIME(ABomb, BombLocation);
	DOREPLIFETIME(ABomb, CountdownSound);
	DOREPLIFETIME(ABomb, BombColor);
}

// ������ �Ӽ��� ����� �� �ڵ����� ȣ��
void ABomb::OnRep_UpdateBombLocation()
{
	// Ŭ���̾�Ʈ�鿡�� ��ġ ������ �˸��� ����
	SetActorLocation(BombLocation);
	UpdateShakeEffect(GetWorld()->GetDeltaSeconds());
	CLog::Log(*BombLocation.ToString());
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
	}
}

void ABomb::Explosion_Implementation()
{
	if (Particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector, true);
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		DestroyBomb();
	}
}

void ABomb::DestroyBomb()
{
	Destroy();
}

void ABomb::StartCountdown()
{
	if (HasAuthority())
	{
		MultiStartCountdown();
	}
}

void ABomb::MultiStartCountdown_Implementation()
{
	TotalCountdownTime = 10.0f;
	ElapseTime = 0.0f;

	if (Audio && CountdownSound)
	{
		Audio->SetSound(CountdownSound);
		Audio->Play();
	}

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ABomb::Explosion, TotalCountdownTime, false);
}

void ABomb::UpDateSoundAndColor(float DeltaTime)
{
	float remainingTime = TotalCountdownTime - ElapseTime;

	float progress = ElapseTime / TotalCountdownTime;
	BombColor = FVector(FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Red, progress));

	if (HasAuthority())
	{
		OnRep_UpdateColor();  // �������� ���� ���� ������Ʈ ȣ��
	}
}

void ABomb::UpdateShakeEffect(float DeltaTime)
{
	float remainingTime = TotalCountdownTime - ElapseTime;
	float shakeIntensity = FMath::Clamp(1.0f - (remainingTime / TotalCountdownTime), 0.0f, 1.0f);
	FVector newLocation = GetActorLocation();
	newLocation.X += FMath::Sin(GetWorld()->GetTimeSeconds() * 20.0f * shakeIntensity) * 5.0f;
	newLocation.Y += FMath::Cos(GetWorld()->GetTimeSeconds() * 10.0f * shakeIntensity) * 5.0f;

	SetActorLocation(newLocation);

	// ��鸲 ȿ�� ��ġ ����
	BombLocation = newLocation;
}

void ABomb::ResetShakeEffect()
{
	ElapseTime = 0.0f;
	TotalCountdownTime = 10.0f;
}

void ABomb::UpdateCountdownWidget(float DeltaTime)
{
	if (CountDownWidget)
	{
		UCountDown* CountdownWidget = Cast<UCountDown>(CountDownWidget->GetWidget());
		if (CountdownWidget)
		{
			int RemainingTime = FMath::CeilToInt(TotalCountdownTime - ElapseTime);
			CountdownWidget->UpdateCountdown(RemainingTime);
		}
	}
}
