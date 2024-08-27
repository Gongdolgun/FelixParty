#include "Components/ParkourComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Components/ArrowComponent.h"

UParkourComponent::UParkourComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    Arrows.SetNum((int32)EParkourArrowType::Max);
    HitResults.SetNum((int32)EParkourArrowType::Max);

    HighStruct.ZOffsetHand = -60.0f;
    HighStruct.ZOffsetLanding = 30.0f;
    HighStruct.MontageLength = 1.1f;

    LowStruct.ZOffsetHand = 5.0f;
    LowStruct.ZOffsetLanding = 20.0f;
    LowStruct.MontageLength = 0.6f;

    JumpStruct.ZOffsetHand = 0.0f;
    JumpStruct.ZOffsetLanding = 30.0f;
    JumpStruct.MontageLength = 0.9f;
}

void UParkourComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return;

    USceneComponent* arrow = Helpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");

    TArray<USceneComponent*> components;
    arrow->GetChildrenComponents(false, components);

    for (int32 i = 0; i < Arrows.Num(); i++)
    {
        Arrows[i] = Cast<UArrowComponent>(components[i]);
    }
}

void UParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UParkourComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, OwnerCharacter);

    DOREPLIFETIME(ThisClass, bCanParkour);
    DOREPLIFETIME(ThisClass, Correction_Height_Relative);

    DOREPLIFETIME(ThisClass, last_TraceAdd1);
    DOREPLIFETIME(ThisClass, last_TraceAdd2);

    DOREPLIFETIME(ThisClass, ParkourPos1);
    DOREPLIFETIME(ThisClass, ParkourPos2);

    DOREPLIFETIME(ThisClass, ParkourRelative);
    DOREPLIFETIME(ThisClass, falling_ImpactPoint);

    DOREPLIFETIME(ThisClass, OutParkourStruct);
}

FVector UParkourComponent::GetParkourPos1()
{
    return ParkourPos1;
}

FVector UParkourComponent::GetParkourPos2()
{
    return ParkourPos2;
}

void UParkourComponent::CorrectPlayerLocation(EParkourType ParkourType)
{
    FVector location = OwnerCharacter->GetActorLocation();
    FVector forward = OwnerCharacter->GetActorForwardVector();

    float AddPlayerLocationZ = 0.0f;
    float addPlayerLocationForward = 0.0f;

    switch (ParkourType)
    {
    case EParkourType::High:
        AddPlayerLocationZ = ParkourRelative.AddPlayerLocationZ_High;
        ParkourRelative.AddPlayerLocationForward = 0.0f;
        break;

    case EParkourType::Low:
        AddPlayerLocationZ = ParkourRelative.AddPlayerLocationZ_Low;
        ParkourRelative.AddPlayerLocationForward = 0.0f;
        addPlayerLocationForward = 0.0f;
        break;

    case EParkourType::Jump:
        AddPlayerLocationZ = ParkourRelative.AddPlayerLocationZ_Jump;
        ParkourRelative.AddPlayerLocationForward = 15.0f;
        break;

    default:
        CLog::Print("Default Error");
        break;
    }

    falling_ImpactPoint = FVector(
        location.X, location.Y, falling_ImpactPoint.Z + AddPlayerLocationZ)
        + FVector(forward * ParkourRelative.AddPlayerLocationForward);

    OwnerCharacter->SetActorLocation(falling_ImpactPoint);
}

void UParkourComponent::SetCanParkour(bool bInCanParkour)
{
    bCanParkour = bInCanParkour;
}

void UParkourComponent::ParkourTrace(FParkourStruct InParkourLocation, float InInitialTraceLength, float InSecondaryTraceZOffset, float InFallingHeightMultiplier)
{
    if (OwnerCharacter == nullptr) return;

    // 초기화
    OutParkourStruct = InParkourLocation;
    OutParkourStruct.ParkourType = EParkourType::Max;

    bool bAllHit = true;       // 모든 방향에서 히트가 발생했는지를 체크
    bool bDownHit = false;      // Down 방향에서의 히트 여부 체크

    // 초기 파쿠르 가능 값은 False
    SetCanParkour(false);

    ParkourPos1 = FVector::ZeroVector;
    ParkourPos2 = FVector::ZeroVector;

    // 라인 트레이스 수행
    LineTrace(EParkourArrowType::Center, InInitialTraceLength);
    LineTrace(EParkourArrowType::Left, InInitialTraceLength);
    LineTrace(EParkourArrowType::Right, InInitialTraceLength);
    LineTrace(EParkourArrowType::Down, InInitialTraceLength);

    // 각 방향의 히트 결과 확인
    for (int32 i = 0; i < Arrows.Num(); i++)
    {
        if (i < HitResults.Num())
        {
            if (i != (int32)EParkourArrowType::Down)
            {
                if (!HitResults[i].bBlockingHit)
                {
                    bAllHit = false;
                }
            }

            else
            {
                bDownHit = HitResults[i].bBlockingHit;
            }
        }
    }

    // 모든 방향에서 충돌이 발생한 경우
    if (bAllHit)
    {
        // 점프중이면 점프 파쿠르
        if (OwnerCharacter->GetCharacterMovement()->IsFalling())
        {
            OutParkourStruct.ParkourType = EParkourType::Jump;
        }

        else
        {
            OutParkourStruct.ParkourType = EParkourType::High;

        }
    }

    // Down 방향에서만 충돌하면서, 점프 중이 아닐 때
    else if (bDownHit && !OwnerCharacter->GetCharacterMovement()->IsFalling())
    {
        // Down 방향에서만 충돌이 발생했는지 확인
        if (!HitResults[(int32)EParkourArrowType::Left].bBlockingHit &&
            !HitResults[(int32)EParkourArrowType::Right].bBlockingHit &&
            !HitResults[(int32)EParkourArrowType::Center].bBlockingHit)
        {
            OutParkourStruct.ParkourType = EParkourType::Low;
        }
        else
        {
            return;
        }
    }

    // 모든 방향이 충돌했을 때 또는 Down 방향만 충돌했을 때
    if ((bAllHit && Check_ObjectRotation()) || (bDownHit && OwnerCharacter->CanJump()))
    {
        ParkourCheck(InSecondaryTraceZOffset, InFallingHeightMultiplier, OutParkourStruct.ParkourType);
    }

    else
    {
        // 오브젝트 충돌이 되지 않았다면 파쿠르가 되지 않도록
        SetCanParkour(false);
    }

    OutParkourStruct.OutLocation1 = ParkourPos1;
    OutParkourStruct.OutLocation2 = ParkourPos2;

}

void UParkourComponent::ParkourCheck(float InSecondaryTraceZOffset, float InFallingHeightMultiplier, EParkourType ParkourType)
{
    FVector falling_Start;
    FVector falling_End;
    EParkourType parkourType = ParkourType;

    bool falling = OwnerCharacter->GetCharacterMovement()->IsFalling();

    // 바닥에 있을 때 Height 값 조절
    float fallingHeightMultiplier = falling ? 1.0f : InFallingHeightMultiplier;

    // 보정값
    float Correction_Height = InSecondaryTraceZOffset * fallingHeightMultiplier;

    if (parkourType == EParkourType::High || parkourType == EParkourType::Jump)
    {
        falling_Start = HitResults[(int32)EParkourArrowType::Center].Location + FVector(0.0f, 0.0f, Correction_Height + Correction_Height_Relative);
        falling_End = HitResults[(int32)EParkourArrowType::Center].Location;
    }

    else if (parkourType == EParkourType::Low)
    {
        falling_Start = HitResults[(int32)EParkourArrowType::Down].Location + FVector(0.0f, 0.0f, Correction_Height + Correction_Height_Relative);
        falling_End = HitResults[(int32)EParkourArrowType::Down].Location;
    }

    FHitResult falling_HitResult;
    TArray<AActor*> falling_ignores;

    UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), falling_Start, falling_End, 10.0f, TraceType, false, falling_ignores, DrawDebug_Parkour, falling_HitResult, true,
        FLinearColor::Blue, FLinearColor::Black);

    // 추적에서 검출된 점을 사용하여 이동 워프할 위치 찾기
    if (falling_HitResult.bBlockingHit)
    {
        falling_ImpactPoint = falling_HitResult.ImpactPoint;

        // Jump
        if (parkourType == EParkourType::Jump)
        {
            // 30 이상이면 High로 변경
            bool bHeightCheck = (falling_ImpactPoint.Z - OwnerCharacter->GetActorLocation().Z) > 50.0f;

            if (bHeightCheck == true)
            {
                OutParkourStruct.ParkourType = EParkourType::High;
                ParkourPos1 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * -10.0f);
            }

            else
            {
                ParkourPos1 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 8.0f);
            }

        }

        // High Or Low
        else
        {
            float Height = falling_ImpactPoint.Z - OwnerCharacter->GetActorLocation().Z;
            CLog::Print(Height);
            bool bHeightCheck = true;
            bHeightCheck &= 20.0f < Height;
            bHeightCheck &= Height < 45.0f;

            // 일반 파쿠르에서 Low와 High 의 사이
            if (bHeightCheck == true)
            {
                OutParkourStruct.ParkourType = EParkourType::Jump;
                ParkourPos1 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 8.0f);
            }

            else
            {
                ParkourPos1 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * -10.0f);
            }


        }

        ParkourPos2 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 120.0f);

        SetCanParkour(true);

        FVector parkour_Start = FVector(ParkourPos2.X, ParkourPos2.Y, ParkourPos2.Z + 20.0f);
        FVector parkour_End = FVector(ParkourPos2.X, ParkourPos2.Y, ParkourPos2.Z + 20.0f);

        FHitResult parkour_HitResult;
        TArray<AActor*> parkour_ignores;

        UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), parkour_Start, parkour_End, 10.0f, TraceType, false, parkour_ignores, DrawDebug_Parkour, parkour_HitResult, true);

        if (parkour_HitResult.bBlockingHit)
        {
            SetCanParkour(false);
        }

        else
        {
            // 워프위치 수정
            ParkourPos2 = falling_HitResult.ImpactPoint + (OwnerCharacter->GetActorForwardVector() * 20.0f);
        }

        // 첫 번째와 두 번째 파쿠르 위치의 경로가 명확한지 확인하기 위한 마지막 Trace
        bool bCheck = false;
        bCheck |= UKismetMathLibrary::EqualEqual_VectorVector(ParkourPos1, FVector::ZeroVector);
        bCheck |= UKismetMathLibrary::EqualEqual_VectorVector(ParkourPos2, FVector::ZeroVector);

        if (bCheck)
        {
            SetCanParkour(false);
        }

        else
        {
            // last_TraceAdd의 기본값은 100으로
            FVector last_Start = FVector(ParkourPos1.X, ParkourPos1.Y, ParkourPos2.Z + last_TraceAdd1);
            FVector last_End = FVector(ParkourPos2.X, ParkourPos2.Y, ParkourPos2.Z + last_TraceAdd2);

            FHitResult last_HitResult;
            TArray<AActor*> last_ignores;

            UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter->GetWorld(), last_Start, last_End, 10.0f, TraceType, false, last_ignores, DrawDebug_Parkour, last_HitResult, true);

            if (last_HitResult.bBlockingHit)
            {
                SetCanParkour(false);
            }
        }
    }
}

void UParkourComponent::LineTrace(EParkourArrowType InType, float InInitialTraceLength)
{
    UArrowComponent* arrow = Arrows[(int32)InType];
    FLinearColor color = FLinearColor(arrow->ArrowColor);

    FTransform transform = arrow->GetComponentToWorld();

    FVector start = transform.GetLocation();
    FVector end = start + (OwnerCharacter->GetActorForwardVector() * InInitialTraceLength);
    TArray<AActor*> ignores;

    // 구의 반지름 설정
    float sphereRadius = 5.0f; // 3cm의 반지름

    // 오브젝트 체크
    UKismetSystemLibrary::SphereTraceSingle(
        OwnerCharacter->GetWorld(),
        start,
        end,
        sphereRadius,
        TraceType,
        false,
        ignores,
        DrawDebug_Parkour,
        HitResults[(int32)InType],
        true,
        color,
        FLinearColor::White
    );


}

bool UParkourComponent::Check_ObjectRotation()
{
    FVector center = HitResults[(int32)EParkourArrowType::Center].Normal;
    FVector left = HitResults[(int32)EParkourArrowType::Left].Normal;
    FVector right = HitResults[(int32)EParkourArrowType::Right].Normal;

    if (center.Equals(left) == false) return false;
    if (center.Equals(right) == false) return false;

    FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
    FVector end = OwnerCharacter->GetActorLocation();
    float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;

    FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
    float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

    float yaw = abs(abs(lookAt) - abs(impactAt));

    if (yaw >= AvailableFrontAngle) return false;

    return true;

}

void UParkourComponent::PlayParkourMontage_NMC_Implementation(EParkourType ParkourType)
{
    UAnimMontage* montage;

    switch (ParkourType)
    {
    case EParkourType::High:
        montage = High_ParkourMontage;
        break;

    case EParkourType::Low:
        montage = Low_ParkourMontage;
        break;

    case EParkourType::Jump:
        montage = Jump_ParkourMontage;
        break;

    default:
        return;
    }

    OwnerCharacter->PlayAnimMontage(montage);
}

void UParkourComponent::PlayParkourMontage_Server_Implementation(EParkourType ParkourType)
{
    PlayParkourMontage_NMC(ParkourType);
}

void UParkourComponent::PlayParkourMontage(EParkourType ParkourType)
{
    if (OwnerCharacter->IsLocallyControlled())
        PlayParkourMontage_Server(ParkourType);
}


