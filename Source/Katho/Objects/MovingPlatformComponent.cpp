// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatformComponent.h"

// Sets default values for this component's properties
UMovingPlatformComponent::UMovingPlatformComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UMovingPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	LastPosition = GetComponentLocation();
}


// Called every frame
void UMovingPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLoc = GetComponentLocation();
	Delta = CurrentLoc - LastPosition;
	LastPosition = CurrentLoc;
}
