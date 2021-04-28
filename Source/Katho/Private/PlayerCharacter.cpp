// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PlayerMovement.h"
#include "UI/PlayerUI.h"
#include "Objects/MovingPlatformComponent.h"
#include "PlayerAnimation.h"

#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <LevelSequence/Public/LevelSequenceActor.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetCollisionProfileName(TEXT("Pawn"));

	MovementComponent = CreateDefaultSubobject<UPlayerMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = Root;

	Model = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Model"));
	Model->SetupAttachment(Root);

	CameraPivot = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Pivot"));
	CameraPivot->SetupAttachment(Root);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraPivot);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (LevelSequenceActor) {
		LevelSequencePlayer = LevelSequenceActor->GetSequencePlayer();
	}

	UUserWidget* Widget = CreateWidget(GetWorld(), PlayerUIBlueprint, TEXT("PlayerUI"));
	PlayerUI = Cast<UPlayerUI>(Widget);
	Widget->AddToViewport();

	if (PlayUIAnimation) {
		PlayerUI->PlayTransitionAnimation(true, false);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LevelSequenceActor) {
		LevelCurrentTime = LevelSequencePlayer->GetCurrentTime().AsSeconds();
		float Duration = LevelSequencePlayer->GetDuration().AsSeconds();
		LevelTimePercent = LevelCurrentTime / Duration;

		PlayerUI->SetTimeSigilPosition(LevelTimePercent, TimeControlOn);
	}

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	float MouseX = GetInputAxisValue("CameraH") / ViewportSize.X * MouseSensitivity;
	float MouseY = GetInputAxisValue("CameraV") / ViewportSize.Y * MouseSensitivity;
	
	// Camera movement
	if (!TimeControlOn) {
		CameraPivot->AddLocalRotation(FRotator(MouseY * 60.f * DeltaTime, MouseX * 60.f * DeltaTime, 0));
		FRotator Rot = CameraPivot->GetComponentRotation();
		Rot.Pitch = FMath::Clamp(Rot.Pitch, -50.f, 40.f);
		CameraPivot->SetWorldRotation(FRotator(Rot.Pitch, Rot.Yaw, 0.f));
	}
	// Time control
	else if (LevelSequenceActor) {
		float Duration = LevelSequencePlayer->GetDuration().AsSeconds();
		float Result = FMath::Clamp(LevelCurrentTime + MouseX * 2.f * DeltaTime, 0.f, Duration);
		LevelSequencePlayer->JumpToSeconds(Result);
	}

	PlatformMotion = FVector::ZeroVector;

	// Jumping
	if (!JumpedThisFrame) {
		FHitResult GroundCheck;
		OnGround = GetWorld()->LineTraceSingleByChannel(GroundCheck, GetActorLocation(), GetActorLocation() - FVector(0, 0, GroundCheckDistance), ECC_Visibility);
		ZVelocity = OnGround ? 0.f : FMath::Max(ZVelocity - Gravity * DeltaTime, -TerminalVelocity);

		// Moving platforms
		if (GroundCheck.GetActor()) {
			auto* Platform = Cast<UMovingPlatformComponent>(GroundCheck.GetActor()->GetComponentByClass(UMovingPlatformComponent::StaticClass()));
			if (Platform) {
				PlatformMotion = Platform->GetDelta();
			}
		}
	}

	JumpedThisFrame = false;

	// Movement
	bool Moving = false;
	if (AllowMovement) {
		float X = GetInputAxisValue("Forward");
		float Y = GetInputAxisValue("Side");

		FVector MotionTarget = FVector::ZeroVector;
		FVector ForwardVec = Camera->GetForwardVector();
		ForwardVec.Z = 0.f;
		ForwardVec.Normalize();
		MotionTarget += ForwardVec * X;
		MotionTarget += Camera->GetRightVector() * Y;
		//MotionTarget.Normalize();

		Motion = FMath::Lerp(Motion, MotionTarget, DeltaTime * 10.f);
		Motion.Z = ZVelocity;

		FVector FinalMotion = Motion * SpeedLimit * DeltaTime;
		FinalMotion += PlatformMotion;
		MovementComponent->AddInputVector(FinalMotion);
		Moving = !MotionTarget.IsZero();

		// Rotation
		if (Moving) {
			FQuat QFrom = Model->GetComponentRotation().Quaternion();
			FQuat QTo{ FVector(0, 0, 1), FMath::Atan2(-MotionTarget.X, MotionTarget.Y) - FMath::DegreesToRadians(180.f) };
			Model->SetWorldRotation(FQuat::Slerp(QFrom, QTo, DeltaTime * 10.f));
		}
	}

	// Animation
	UPlayerAnimation* AnimInstance = Cast<UPlayerAnimation>(Model->GetAnimInstance());
	AnimInstance->Moving = Moving;
	AnimInstance->RunStrength = Motion.Size2D();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward");
	PlayerInputComponent->BindAxis("Side");
	PlayerInputComponent->BindAxis("CameraH");
	PlayerInputComponent->BindAxis("CameraV");

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("TimeControl", IE_Pressed, this, &APlayerCharacter::StartTimeControl);
	PlayerInputComponent->BindAction("TimeControl", IE_Released, this, &APlayerCharacter::EndTimeControl);
}


void APlayerCharacter::FinishLevel(const FName& NextLevel)
{
	AllowMovement = false;
	PlayerUI->SetTargetNewLevel(NextLevel);
	PlayerUI->PlayTransitionAnimation(false, true);
}


void APlayerCharacter::Jump()
{
	if (OnGround) {
		ZVelocity = JumpStrength;
		JumpedThisFrame = true;
	}
}


void APlayerCharacter::StartTimeControl()
{
	if (LevelSequenceActor) {
		LevelSequencePlayer->Pause();
		PlayerUI->PlayTimeDisplayFadeAnimation(false);
		PlayerUI->PlaySigilAnimation(false);
		TimeControlOn = true;
	}
}


void APlayerCharacter::EndTimeControl()
{
	if (LevelSequenceActor) {
		LevelSequencePlayer->Play();
		PlayerUI->PlayTimeDisplayFadeAnimation(true);
		PlayerUI->PlaySigilAnimation(true);
		TimeControlOn = false;
	}
}
