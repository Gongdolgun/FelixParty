#include "SpawnActor/TargetDecal.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Components/DecalComponent.h"

ATargetDecal::ATargetDecal()
{
	MouseDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	MouseDecal->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/GameTypes/Bomb/Material/M_Crack_Inst.M_Crack_Inst'"));
	if (DecalMaterialAsset.Succeeded())
	{
		MouseDecal->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	MouseDecal->DecalSize = FVector(64.0f, 64.0f, 64.0f);
	MouseDecal->SetFadeScreenSize(0.1f);
	MouseDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	MouseDecal->SetSortOrder(1); // Sort Order ¼³Á¤

	//SetActorHiddenInGame(true);
}

void ATargetDecal::BeginPlay()
{
	Super::BeginPlay();

	MouseDecal = Cast<UDecalComponent>(RootComponent);
	if (MouseDecal)
	{
		UMaterialInstanceDynamic* DecalMaterial = UMaterialInstanceDynamic::Create(MouseDecal->GetMaterial(0), this);
		if (DecalMaterial)
		{
			DecalMaterial->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
			MouseDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void ATargetDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetDecal::UpdateDecalLocation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
}
