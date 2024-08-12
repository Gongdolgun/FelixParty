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
        // ������ �̵�
        FVector NewLocation = GetActorLocation() + (GetActorForwardVector() * MoveSpeed * DeltaTime);
        SetActorLocation(NewLocation);

        // �̵� �Ÿ��� �����ϸ� ���ƿ��� ����
        if (FVector::Dist(StartLocation, GetActorLocation()) >= MoveDistance)
        {
            bIsMovingForward = false; // �̵� ���� ����
            GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &APunchObject::StartMoving, RepeatDelay, false);
        }
    }
    else
    {
        // ���� ��ġ�� ���ƿ���
        FVector DirectionToStart = (StartLocation - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + (DirectionToStart * ReturnSpeed * DeltaTime);
        SetActorLocation(NewLocation);

        // ���� ��ġ�� �����ϸ� ����
        if (FVector::Dist(StartLocation, GetActorLocation()) <= 1.0f) // �ణ�� ������ �ΰ� üũ
        {
            SetActorLocation(StartLocation); // ��Ȯ�� ���� ��ġ�� ����
        }
    }
}

void APunchObject::StartMoving()
{
    bIsMovingForward = true; // �̵� ����
}