#pragma once

#include "CoreMinimal.h"
#include "BlueprintDataDefinitions.h"
#include "Materials/MaterialInterface.h"
#include "Enums.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FPlayerBaseInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReady = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBPUniqueNetId UniqueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName PlayerName = FName(TEXT("Player Name"));
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> CharacterMaterials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBPUniqueNetId UniqueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName PlayerName = FName(TEXT("Player Name"));

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int TotalScore = 0;
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
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 연사 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float FireRate;

    // 탄 튀는 정도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float RecoilAngle;

    // 카메라 셰이크
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    TSubclassOf<UCameraShakeBase> CameraShakeBase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float ShakeVelocity = 1.f;

    // 플래쉬 파티클
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* FlashParticle;

    // 탄피 파티클
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* EjectionParticle;

    // 사물에 맞았을 때 생성될 파티클
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* HitParticle_Props;

    // 캐릭터에 맞았을 때 생성될 파티클
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* HitParticle_Character;

    // 트레이스 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float HitDistance = 3000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    TSubclassOf<AActor> Bullet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    int MaxBullet;

    // 사격 사운드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    class USoundWave* Sound = nullptr;

};

USTRUCT(BlueprintType)
struct FPlayerInGameData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly)
    int32 Score;

    UPROPERTY(BlueprintReadOnly)
    FBPUniqueNetId UniqueID;

    FPlayerInGameData()
        : PlayerName(TEXT(""))
        , Score(0)
    {}

    FPlayerInGameData(const FString& InPlayerName, int32 InScore)
        : PlayerName(InPlayerName)
        , Score(InScore)
    {}

    FPlayerInGameData(const FString& InPlayerName, int32 InScore, FBPUniqueNetId InUniqueID)
        : PlayerName(InPlayerName)
        , Score(InScore)
		, UniqueID(InUniqueID)
    {}
};