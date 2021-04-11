// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class KATHO_API APlayerCharacter : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float SpeedLimit = 500.f;
	UPROPERTY(EditAnywhere)
	float Gravity = 7.4f;
	UPROPERTY(EditAnywhere)
	float JumpStrength = 2.8f;

	static constexpr float GroundCheckDistance = 58.f;

	float TerminalVelocity = 53.f;
	float ZVelocity = 0.f;

	FVector Motion = FVector::ZeroVector;
	bool OnGround = false;
	bool JumpedThisFrame = false;
	bool AllowMovement = true;
	FVector PlatformMotion = FVector::ZeroVector;

	bool TimeControlOn = false;
	float LevelCurrentTime;
	float LevelTimePercent;

	UPROPERTY(EditDefaultsOnly, DisplayName = "Player UI Blueprint")
	TSubclassOf<class UUserWidget> PlayerUIBlueprint;
	class UPlayerUI* PlayerUI;

	UPROPERTY(EditAnywhere)
	class ALevelSequenceActor* LevelSequenceActor;
	class ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Root;
	UPROPERTY(VisibleAnywhere)
	class UPlayerMovement* MovementComponent;
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Model;
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraPivot;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
public:
	// Sets default values for this pawn's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StartTimeControl();
	UFUNCTION()
	void EndTimeControl();
	//UFUNCTION()
	//void TimeControlTick();
};
