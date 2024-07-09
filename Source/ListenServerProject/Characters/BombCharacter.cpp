#include "Characters/BombCharacter.h"
#include "Utilites/Helpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABombCharacter::ABombCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/*Helpers::CreateComponent<USpringArmComponent>(this, &TopDownSpringArm, "TopDownSpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<UCameraComponent>(this, &TopDownCamera, "TopDownSpringArm", TopDownSpringArm);*/

	TopDownSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TopDownSpringArm"));
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownSpringArm->SetupAttachment(RootComponent);
	TopDownSpringArm->SetUsingAbsoluteRotation(true); // 절대 회전값 사용
	TopDownSpringArm->TargetArmLength = 1000.f;
	TopDownSpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	TopDownSpringArm->bUsePawnControlRotation = false;
	TopDownSpringArm->bDoCollisionTest = false;

	TopDownCamera->SetupAttachment(TopDownSpringArm, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	//SpringArm->SetupAttachment(RootComponent);
	//SpringArm->SetUsingAbsoluteRotation(true); // 절대 회전값 사용
	//SpringArm->TargetArmLength = 1000.f;
	//SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	//SpringArm->bUsePawnControlRotation = false;
	//SpringArm->bDoCollisionTest = false;

	//Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	//Camera->bUsePawnControlRotation = false;
}

void ABombCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABombCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABombCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABombCharacter::Action()
{
	Super::Action();
}
