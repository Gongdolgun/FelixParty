#include "Characters/LobbyCharacter.h"
#include "EnhancedInputComponent.h"
#include "Global.h"
#include "Components/MoveComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneCaptureComponent.h"
#include "Controllers/LobbyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/LobbyGameMode.h"
#include "GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"

ALobbyCharacter::ALobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	Helpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<USceneCaptureComponent2D>(this, &SceneCaptureCamera, "SceneCaptureCamera", SpringArm);

	SpringArm->SetRelativeRotation(FRotator(0, 180, 0));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bDoCollisionTest = false;

	SceneCaptureCamera->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureCamera->FOVAngle = 46.f;
}

void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SceneCaptureCamera->ShowOnlyActors.Add(this);
	SetTextureTarget_Server();
}

void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALobbyCharacter::ChangeMaterial(FColor InColor)
{
	if (HasAuthority())
		ChangeMaterial_NMC(InColor);

	else
		ChangeMaterial_Server(InColor);
}

void ALobbyCharacter::PlayReadyMontage_NMC_Implementation(UAnimMontage* InMontage)
{
	PlayAnimMontage(InMontage);
}

void ALobbyCharacter::PlayReadyMontage_Server_Implementation(UAnimMontage* InMontage)
{
	PlayReadyMontage_NMC(InMontage);
}

void ALobbyCharacter::PlayReadyMontage(UAnimMontage* InMontage)
{
	PlayReadyMontage_Server(InMontage);
}

void ALobbyCharacter::SetTextureTarget_Server_Implementation()
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());

	if(LobbyGameMode != nullptr)
	{
		LobbyGameMode->UpdatePlayerTextureRender();
	}
}

void ALobbyCharacter::SetTextureTarget_NMC_Implementation(int32 InNumber)
{
	SceneCaptureCamera->TextureTarget = TextureRenderTargets[InNumber];
}

void ALobbyCharacter::UpdatePlayer_Server_Implementation()
{
	if (ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode()))
		LobbyGameMode->UpdatePlayerMaterial();
}

void ALobbyCharacter::ChangeMaterial_Server_Implementation(FColor InColor)
{
	ChangeMaterial_NMC(InColor);
}

void ALobbyCharacter::ChangeMaterial_NMC_Implementation(FColor InColor)
{
	int32 MaterialCount = GetMesh()->GetNumMaterials();
	for(int32 i = 0; i < MaterialCount; i++)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(i));
		if (!MaterialInstance)
			MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);

		if (MaterialInstance)
		{
			if(MaterialInstance)
				MaterialInstance->SetVectorParameterValue(FName("Tint"), InColor);
		}
	}

	if (OverlayMaterial != nullptr)
	{
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(OverlayMaterial, this);

		if (MaterialInstance)
		{
			MaterialInstance->SetVectorParameterValue(FName("Color"), FLinearColor(InColor));
			GetMesh()->SetOverlayMaterial(MaterialInstance);
		}
	}
}

void ALobbyCharacter::AddSelectedColor_Implementation(const FString& InColor)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(LobbyGameMode->GetGameState<ALobbyGameState>());
		if (LobbyGameState)
		{
			if (!LobbyGameState->SelectedColors.Contains(InColor))
				LobbyGameState->SelectedColors.Add(InColor);
		}
	}
}

void ALobbyCharacter::RemoveSelectedColor_Implementation(const FString& InColor)
{
	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(LobbyGameMode->GetGameState<ALobbyGameState>());
		if (LobbyGameState)
		{
			if (LobbyGameState->SelectedColors.Contains(InColor))
				LobbyGameState->SelectedColors.Remove(InColor);
		}
	}
}

