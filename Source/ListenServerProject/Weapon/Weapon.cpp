#include "Weapon/Weapon.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Characters/DefaultCharacter.h"
#include "Components/WeaponComponent.h"
#include "Controllers/DefaultController.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	// 초기 무기 Attach
	if (AttachSocketName.IsValid())
		Helpers::AttachTo(this, Owner->GetMesh(), AttachSocketName);

	if(WeaponDataTable)
		WeaponData = *WeaponDataTable->FindRow<FWeaponData>(WeaponName, TEXT("Data Setting"));

}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Equip()
{

}

void AWeapon::Fire()
{
	UCameraComponent* camera = Helpers::GetComponent<UCameraComponent>(Owner);
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, WeaponData.RecoilAngle);
	FVector end = transform.GetLocation() + direction * WeaponData.HitDistance;

	TArray<AActor*> ignores;
	ignores.Add(Owner);

	FHitResult hitResult;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTyps;
	TEnumAsByte<EObjectTypeQuery> CharacterMesh = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1);
	ObjectTyps.Add(CharacterMesh);
	ObjectTyps.Add(WorldStatic);

	// 카메라 셰이크
	if (WeaponData.CameraShakeBase)
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

	// 탄피, 플래쉬 파티클 생성
	if (WeaponData.FlashParticle)
		UGameplayStatics::SpawnEmitterAttached(WeaponData.FlashParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	if (WeaponData.EjectionParticle)
		UGameplayStatics::SpawnEmitterAttached(WeaponData.EjectionParticle, Mesh, "Eject", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	// 사운드 플레이
	if (WeaponData.Sound)
	{
		FVector SoundLocation = Mesh->GetSocketLocation("Muzzle");
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponData.Sound, SoundLocation);
	}

	// 총알 생성
	if (WeaponData.Bullet)
	{
		FVector location = Mesh->GetSocketLocation("Bullet");

		FActorSpawnParameters params;
		params.Owner = Owner;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* bullet = GetWorld()->SpawnActor<AActor>(WeaponData.Bullet, location, direction.Rotation(), params);
	}

	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), start, end, ObjectTyps, false, ignores, EDrawDebugTrace::None, hitResult, true))
	{
		ADefaultCharacter* HittedCharacter = Cast<ADefaultCharacter>(hitResult.GetActor());

		// 헤드 맞추면 데미지 100, 아니면 HitData의 Damage
		if(HittedCharacter)
		{
			if(hitResult.BoneName == "head")
			{
				FHitData HeadShotData = HitData;
				HeadShotData.Damage = 100.f;

				HittedCharacter->Hit(Owner, HeadShotData);
			}

			else
				HittedCharacter->Hit(Owner, HitData);

			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.HitParticle_Character, hitResult.Location, rotator);
		}

		else
		{
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData.HitParticle_Props, hitResult.Location, rotator);
		}
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
	// TODO :: 장착해제 -> 무기 사라지게 만들기
}

