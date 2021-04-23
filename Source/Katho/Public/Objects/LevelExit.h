// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelExit.generated.h"

UCLASS()
class KATHO_API ALevelExit : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName NextLevel;
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Collider;
	
public:	
	// Sets default values for this actor's properties
	ALevelExit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void PlayerEnterArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
