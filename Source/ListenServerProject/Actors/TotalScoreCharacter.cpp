#include "Actors/TotalScoreCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Global.h"
#include "GameState/DefaultGameState.h"

ATotalScoreCharacter::ATotalScoreCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Helpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<USceneCaptureComponent2D>(this, &SceneCaptureCamera, "SceneCaptureCamera", SpringArm);

	SpringArm->SetRelativeRotation(FRotator(0, 180, 0));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bDoCollisionTest = false;

	SceneCaptureCamera->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureCamera->FOVAngle = 46.f;
}

void ATotalScoreCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultGameState = Cast<ADefaultGameState>(GetWorld()->GetGameState());
	if(DefaultGameState != nullptr)
	{
		DefaultGameState->OnGameStateTypeChanged.AddDynamic(this, &ATotalScoreCharacter::OnGameStateTypeChanged);
	}

	if (OverlayMaterial != nullptr)
		OverlayMaterialDynamic = UMaterialInstanceDynamic::Create(OverlayMaterial, this);

	int32 MaterialCount = GetMesh()->GetNumMaterials();
	for (int32 i = 0; i < MaterialCount; i++)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(i));
		if (!MaterialInstance)
			MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);

		if (MaterialInstance)
			MyMaterials.Add(MaterialInstance);
	}

	DefaultGameState = Cast<ADefaultGameState>(GetWorld()->GetGameState());

	SceneCaptureCamera->ShowOnlyActors.Add(this);
	//GetMesh()->SetHiddenInGame(true);
}

void ATotalScoreCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlayMaterialDynamic != nullptr && DefaultGameState != nullptr)
	{
		OverlayMaterialDynamic->SetVectorParameterValue(FName("Color"), FLinearColor(DefaultGameState->PlayerDatas[Number].PlayerColor));
		GetMesh()->SetOverlayMaterial(OverlayMaterialDynamic);
	}

	if(!MyMaterials.IsEmpty() && DefaultGameState != nullptr)
	{
		for(auto MyMaterial : MyMaterials)
		{
			MyMaterial->SetVectorParameterValue(FName("Tint"), DefaultGameState->PlayerDatas[Number].PlayerColor);
		}
	}
}

void ATotalScoreCharacter::OnGameStateTypeChanged(EGameStateType InPrevGameType, EGameStateType InNewGameType)
{
	if(InNewGameType == EGameStateType::TotalRankBoard)
	{
		if(!AnimMontages.IsEmpty())
		{
			int32 RandomInteger = UKismetMathLibrary::RandomIntegerInRange(0, AnimMontages.Num() - 1);
			//GetMesh()->SetHiddenInGame(false);
			PlayanimMontage_NMC(AnimMontages[RandomInteger]);
		}
	}
}

void ATotalScoreCharacter::Init()
{
	if(!TextureRenderTargets.IsEmpty())
	{
		SceneCaptureCamera->TextureTarget = TextureRenderTargets[Number];
	}
}

void ATotalScoreCharacter::PlayanimMontage_NMC_Implementation(UAnimMontage* InAnimMontage)
{
	PlayAnimMontage(InAnimMontage);
}