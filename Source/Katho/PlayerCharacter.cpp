// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PlayerMovement.h"

#include <GameFramework/SpringArmComponent.h>
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Camera/CameraComponent.h>

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
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Camera movement
	CameraPivot->AddLocalRotation(FRotator(GetInputAxisValue("CameraV") * 60.f * DeltaTime, GetInputAxisValue("CameraH") * 60.f * DeltaTime, 0));
	FRotator Rot = CameraPivot->GetComponentRotation();
	CameraPivot->SetWorldRotation(FRotator(Rot.Pitch, Rot.Yaw, 0.f));

	if (!JumpedThisFrame) {
		FHitResult GroundCheck;
		OnGround = GetWorld()->LineTraceSingleByChannel(GroundCheck, GetActorLocation(), GetActorLocation() - FVector(0, 0, GroundCheckDistance), ECC_Visibility);
		ZVelocity = OnGround ? 0.f : FMath::Max(ZVelocity - Gravity * DeltaTime, -TerminalVelocity);
	}

	JumpedThisFrame = false;

	// Movement
	bool Moving = false;
	if (AllowMovement) {
		float X = GetInputAxisValue("Forward");
		float Y = GetInputAxisValue("Side");

		FTransform CamXform = Camera->GetComponentTransform();
		FVector MotionTarget = FVector::ZeroVector;
		MotionTarget += CamXform.GetRotation().GetAxisY() * Y;
		MotionTarget += CamXform.GetRotation().GetAxisX() * X;
		MotionTarget.Normalize();

		Motion = FMath::Lerp(Motion, MotionTarget, DeltaTime * 10.f);
		Motion.Z = ZVelocity;

		FVector FinalMotion = Motion * SpeedLimit * DeltaTime;
		MovementComponent->AddInputVector(FinalMotion);
		Moving = !MotionTarget.IsZero();

		// Rotation
		if (Moving) {
			FQuat QFrom = Model->GetComponentRotation().Quaternion();
			FQuat QTo{ FVector(0, 0, 1), FMath::Atan2(-MotionTarget.X, MotionTarget.Y) + FMath::DegreesToRadians(90.f) };
			Model->SetWorldRotation(FQuat::Slerp(QFrom, QTo, DeltaTime * 10.f));
		}
	}

	// Do animation here
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
}


void APlayerCharacter::Jump()
{
	if (OnGround) {
		ZVelocity = JumpStrength;
		JumpedThisFrame = true;
	}
}
