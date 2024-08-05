#include "Weapon/Weapon.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Characters/DefaultCharacter.h"
#include "Characters/FPSCharacter.h"
#include "Components/WeaponComponent.h"
#include "Controllers/DefaultController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AFPSCharacter>(GetOwner());
	if (Owner == nullptr)
		return;

	// �ʱ� ���� Attach
	if (AttachSocketName.IsValid())
		Helpers::AttachTo(this, Owner->GetMesh(), AttachSocketName);

	if(WeaponDataTable != nullptr)
		WeaponData = *WeaponDataTable->FindRow<FWeaponData>(WeaponName, TEXT("Data Setting"));

	MaxBullet = WeaponData.MaxBullet;
	CurBullet = MaxBullet;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Equip()
{

}

void AWeapon::BeginFire()
{
	Fire();
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ThisClass::Fire, WeaponData.FireRate, true);
}

void AWeapon::EndFire()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void AWeapon::Fire()
{
	if (Owner != nullptr && CurBullet != 0 && !Owner->GetMovementComponent()->IsFalling())
	{
		Owner->SeperateServer(WeaponData, HitData);
		CurBullet -= 1;
	}
}

void AWeapon::Fire_Event(FVector direction, FHitResult HitResult)
{
	// �ǰݵ� ��� ���� ��ƼŬ ����
	ADefaultCharacter* HittedCharacter = Cast<ADefaultCharacter>(HitResult.GetActor());
	if (HittedCharacter != nullptr)
	{
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(HitResult.Location, HitResult.TraceStart);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.HitParticle_Character, HitResult.Location, rotator);
	}
	else
	{
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(HitResult.Location, HitResult.TraceStart);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.HitParticle_Props, HitResult.Location, rotator);
	}

	// �Ѿ� ����
	if (WeaponData.Bullet != nullptr)
	{
		FVector location = Mesh->GetSocketLocation("Bullet");

		FActorSpawnParameters params;
		params.Owner = Owner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* bullet = GetWorld()->SpawnActor<AActor>(WeaponData.Bullet, location, direction.Rotation(), params);
	}

	// ī�޶� ����ũ
	if (WeaponData.CameraShakeBase != nullptr)
	{
		ADefaultController* Controller = Cast<ADefaultController>(Owner->GetController());

		if (Controller)
		{
			if (Controller->IsLocalController())
			{
				Controller->ClientStartCameraShake(WeaponData.CameraShakeBase, WeaponData.ShakeVelocity);
			}
		}
	}

	// ź��, �÷��� ��ƼŬ ����
	if (WeaponData.FlashParticle != nullptr)
		UGameplayStatics::SpawnEmitterAttached(WeaponData.FlashParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	if (WeaponData.EjectionParticle != nullptr)
		UGameplayStatics::SpawnEmitterAttached(WeaponData.EjectionParticle, Mesh, "Eject", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	// ���� �÷���
	if (WeaponData.Sound != nullptr)
	{
		FVector SoundLocation = Mesh->GetSocketLocation("Muzzle");
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponData.Sound, SoundLocation);
	}
}

void AWeapon::Begin_Equip()
{

}

void AWeapon::End_Equip()
{

}

void AWeapon::UnEquip()
{
	// TODO :: �������� -> ���� ������� �����
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
