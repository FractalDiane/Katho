// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

/**
 * 
 */
UCLASS()
class KATHO_API UPlayerAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Moving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunStrength = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FidgetCounter = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PlayFidget = false;
};
