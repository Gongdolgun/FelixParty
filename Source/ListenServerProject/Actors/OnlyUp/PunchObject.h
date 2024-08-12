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

	// 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    // 돌아오는 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ReturnSpeed;

    // 이동 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveDistance;

    // 반복 주기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RepeatDelay;

private:
    // 이동 상태
    bool bIsMovingForward;

    // 시작 위치
    FVector StartLocation;

    // 타이머 핸들
    FTimerHandle MoveTimerHandle;

    // 이동 시작 함수
    void StartMoving();
};

