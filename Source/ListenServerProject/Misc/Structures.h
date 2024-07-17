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
    //Character���� ������ ������ ��ġ
    UPROPERTY(EditAnywhere, Category = "Damage")
    float Damage = 0.0f;

    //Effect: �ǰ� �� ������ ����Ʈ
    UPROPERTY(EditAnywhere, Category = "Effect")
    class UFXSystemAsset* Effect = nullptr;

    //EffectScale: �ǰ� �� ������ ����Ʈ ũ�� ����
    UPROPERTY(EditAnywhere, Category = "Effect")
    FVector EffectScale = FVector(1.0f, 1.0f, 1.0f);

    //Location: �ǰ� �� ������ ����Ʈ�� ���� ��ġ
    UPROPERTY(EditAnywhere, Category = "Effect")
    FVector Location = FVector(0.0f, 0.0f, 0.0f);

    //Sound: �ǰ� �� ������ ����
    UPROPERTY(EditAnywhere, Category = "Sound")
    class USoundWave* Sound = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // ���� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float FireRate;

    // ź Ƣ�� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float RecoilAngle;

    // ī�޶� ����ũ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    TSubclassOf<UCameraShakeBase> CameraShakeBase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float ShakeVelocity = 1.f;

    // �÷��� ��ƼŬ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* FlashParticle;

    // ź�� ��ƼŬ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* EjectionParticle;

    // �繰�� �¾��� �� ������ ��ƼŬ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* HitParticle_Props;

    // ĳ���Ϳ� �¾��� �� ������ ��ƼŬ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    UParticleSystem* HitParticle_Character;

    // Ʈ���̽� �Ÿ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    float HitDistance = 3000.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    TSubclassOf<AActor> Bullet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FWeaponData")
    int MaxBullet;

    // ��� ����
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