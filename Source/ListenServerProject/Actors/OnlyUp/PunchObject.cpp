#include "Actors/OnlyUp/PunchObject.h"
#include "Components/StaticMeshComponent.h"
#include "Global.h"

APunchObject::APunchObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	Helpers::CreateComponent<UStaticMeshComponent>(this, &StaticMesh, "StaticMesh", Root);

    MoveSpeed = 1000.0f; 
    ReturnSpeed = 200.0f; 
    MoveDistance = 500.0f; 
    RepeatDelay = 3.0f; 
    bIsMovingForward = false; 

}

void APunchObject::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
    StartMoving();
}

void APunchObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsMovingForward)
    {
        // 앞으로 이동
        FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * MoveSpeed * DeltaTime);
        SetActorLocation(NewLocation);

        // 이동 거리가 도달하면 돌아오기 시작
        if (FVector::Dist(StartLocation, GetActorLocation()) >= MoveDistance)
        {
            bIsMovingForward = false; // 이동 상태 변경
            GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &APunchObject::StartMoving, RepeatDelay, false);
        }
    }
    else
    {
        // 원래 위치로 돌아오기
        FVector DirectionToStart = (StartLocation - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + (DirectionToStart * ReturnSpeed * DeltaTime);
        SetActorLocation(NewLocation);

        // 원래 위치에 도달하면 정지
        if (FVector::Dist(StartLocation, GetActorLocation()) <= 1.0f) // 약간의 오차를 두고 체크
        {
            SetActorLocation(StartLocation); // 정확히 원래 위치로 설정
        }
    }
}

void APunchObject::StartMoving()
{
    bIsMovingForward = true; // 이동 시작
}