#include "SpawnActor/TargetDecal.h"
#include "Global.h"
#include "Characters/BombCharacter.h"
#include "Components/DecalComponent.h"

ATargetDecal::ATargetDecal()
{
	UDecalComponent* DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	RootComponent = DecalComponent;

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("/Script/Engine.Material'/Game/GameTypes/Bomb/Decal/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		DecalComponent->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	DecalComponent->DecalSize = FVector(64.0f, 64.0f, 64.0f);
	DecalComponent->SetFadeScreenSize(0.1f);
	DecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	DecalComponent->SetSortOrder(1); // Sort Order ����

	SetActorHiddenInGame(true);
}

void ATargetDecal::BeginPlay()
{
	Super::BeginPlay();

	// RootComponent�� UDecalComponent�� ĳ����
	UDecalComponent* DecalComponent = Cast<UDecalComponent>(RootComponent);
	if (DecalComponent)
	{
		// DecalComponent�� ��Ƽ������ ������� ���� ��Ƽ���� �ν��Ͻ� ����
		UMaterialInstanceDynamic* DecalMaterial = UMaterialInstanceDynamic::Create(DecalComponent->GetMaterial(0), this);
		if (DecalMaterial)
		{
			DecalMaterial->SetScalarParameterValue(TEXT("Opacity"), 1.0f);
			DecalComponent->SetMaterial(0, DecalMaterial);
		}
	}
}

void ATargetDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
