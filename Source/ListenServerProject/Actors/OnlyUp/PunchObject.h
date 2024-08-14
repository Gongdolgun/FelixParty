#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PunchObject.generated.h"

UCLASS()
class LISTENSERVERPROJECT_API APunchObject : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Root")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Static Mesh")
	class UStaticMeshComponent* StaticMesh;

public:	
	APunchObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// �̵� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    // ���ƿ��� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ReturnSpeed;

    // �̵� �Ÿ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveDistance;

    // �ݺ� �ֱ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RepeatDelay;

private:
    // �̵� ����
    bool bIsMovingForward;

    // ���� ��ġ
    FVector StartLocation;

    // Ÿ�̸� �ڵ�
    FTimerHandle MoveTimerHandle;

    // �̵� ���� �Լ�
    void StartMoving();
};

