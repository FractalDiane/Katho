// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovement.h"

void UPlayerMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Motion = ConsumeInputVector();
	if (!Motion.IsNearlyZero()) {
		FHitResult Hit;
		SafeMoveUpdatedComponent(Motion, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit()) {
			SlideAlongSurface(Motion, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}
