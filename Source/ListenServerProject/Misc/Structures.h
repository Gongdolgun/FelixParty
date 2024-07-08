#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FPlayerBaseInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PlayerName = FName(TEXT("Player Name"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* UserThumbnail;
};

USTRUCT(BlueprintType)
struct FPlayerInGameData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> CharacterMaterials;
};

USTRUCT(BlueprintType)
struct FHitData
{
    GENERATED_BODY()

public:
    //Character에게 입히는 데미지 수치
    UPROPERTY(EditAnywhere, Category = "Damage")
    float Damage = 0.0f;

    //Effect: 피격 시 나오는 이펙트
    UPROPERTY(EditAnywhere, Category = "Effect")
    class UFXSystemAsset* Effect = nullptr;

    //EffectScale: 피격 시 나오는 이펙트 크기 비율
    UPROPERTY(EditAnywhere, Category = "Effect")
    FVector EffectScale = FVector(1.0f, 1.0f, 1.0f);

    //Location: 피격 시 나오는 이펙트와 사운드 위치
    UPROPERTY(EditAnywhere, Category = "Effect")
    FVector Location = FVector(0.0f, 0.0f, 0.0f);

    //Sound: 피격 시 나오는 사운드
    UPROPERTY(EditAnywhere, Category = "Sound")
    class USoundWave* Sound = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()

public:
    // 연사 속도
    UPROPERTY(EditAnywhere, Category = "Fire")
    float FireRate;

    // 탄 튀는 정도
    UPROPERTY(EditAnywhere, Category = "Fire")
    float RecoilAngle;

    // 카메라 셰이크
    UPROPERTY(EditAnywhere, Category = "Fire")
    TSubclassOf<UCameraShakeBase> CameraShakeBase;

    UPROPERTY(EditAnywhere, Category = "Fire")
    float ShakeVelocity = 1.f;

    // 플래쉬 파티클
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* FlashParticle;

    // 탄피 파티클
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* EjectionParticle;

    // 사물에 맞았을 때 생성될 파티클
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* HitParticle_Props;

    // 캐릭터에 맞았을 때 생성될 파티클
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* HitParticle_Character;

    UPROPERTY(EditAnywhere, Category = "Bullet")
    TSubclassOf<AActor> Bullet;

    // 트레이스 거리
    UPROPERTY(EditAnywhere, Category = "Sound")
    float HitDistance = 3000.f;

    // 사격 사운드
    UPROPERTY(EditAnywhere, Category = "Sound")
    class USoundWave* Sound = nullptr;
};