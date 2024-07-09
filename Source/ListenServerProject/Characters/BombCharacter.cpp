#include "Characters/BombCharacter.h"
#include "Utilites/Helpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "SpawnActor/Bomb.h"

ABombCharacter::ABombCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	/*Helpers::CreateComponent<USpringArmComponent>(this, &TopDownSpringArm, "TopDownSpringArm", GetCapsuleComponent());
	Helpers::CreateComponent<UCameraComponent>(this, &TopDownCamera, "TopDownSpringArm", TopDownSpringArm);*/

	HandSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HandSphere"));
	HandSphere->InitSphereRadius(20.0f);
	HandSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HandSphere->SetCollisionResponseToAllChannels(ECR_Overlap);

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

	PlayerCharacter = Cast<ACharacter>(GetOwner());

	// Hand_R_Sphere에 SphereComponent 장착
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		HandSphere->AttachToComponent(MeshComp, AttachmentRules, TEXT("Hand_R_Sphere"));
	}

	HandSphere->OnComponentBeginOverlap.AddDynamic(this, &ABombCharacter::OnSphereBeginOverlap);

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

void ABombCharacter::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && BombClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision Detected with %s"), *OtherActor->GetName());

		FVector headLocation = OtherActor->GetActorLocation() + (OtherActor->GetActorUpVector() * 200);

		FRotator spawnRotation = FRotator::ZeroRotator;

		ABomb* spawnBomb = GetWorld()->SpawnActor<ABomb>(BombClass, headLocation, spawnRotation);

		if (spawnBomb)
		{
			spawnBomb->AttachToComponent(OtherActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

			GetWorldTimerManager().SetTimer(BombTimerHandle, [spawnBomb]()
				{
					if (spawnBomb)
					{
						spawnBomb->Destroy();
					}
				}, 5.0f, false);
		}
	}
}

