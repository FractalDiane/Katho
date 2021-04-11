// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MovingPlatformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KATHO_API UMovingPlatformComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	FVector LastPosition;
	FVector Delta;

public:	
	// Sets default values for this component's properties
	UMovingPlatformComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	inline const FVector& GetDelta() { return Delta; }
};
