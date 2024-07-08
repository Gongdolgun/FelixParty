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
struct FWeaponData
{
    GENERATED_BODY()

public:
    // ���� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Fire")
    float FireRate;

    // ź Ƣ�� ����
    UPROPERTY(EditAnywhere, Category = "Fire")
    float RecoilAngle;

    // ī�޶� ����ũ
    UPROPERTY(EditAnywhere, Category = "Fire")
    TSubclassOf<UCameraShakeBase> CameraShakeBase;

    UPROPERTY(EditAnywhere, Category = "Fire")
    float ShakeVelocity = 1.f;

    // �÷��� ��ƼŬ
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* FlashParticle;

    // ź�� ��ƼŬ
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* EjectionParticle;

    // �繰�� �¾��� �� ������ ��ƼŬ
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* HitParticle_Props;

    // ĳ���Ϳ� �¾��� �� ������ ��ƼŬ
    UPROPERTY(EditAnywhere, Category = "Fire")
    UParticleSystem* HitParticle_Character;

    UPROPERTY(EditAnywhere, Category = "Bullet")
    TSubclassOf<AActor> Bullet;

    // Ʈ���̽� �Ÿ�
    UPROPERTY(EditAnywhere, Category = "Sound")
    float HitDistance = 3000.f;

    // ��� ����
    UPROPERTY(EditAnywhere, Category = "Sound")
    class USoundWave* Sound = nullptr;
};