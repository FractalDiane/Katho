// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/LevelExit.h"

#include "PlayerCharacter.h"

#include <Components/BoxComponent.h>

// Sets default values
ALevelExit::ALevelExit()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALevelExit::BeginPlay()
{
	Super::BeginPlay();
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ALevelExit::PlayerEnterArea);
}


void ALevelExit::PlayerEnterArea(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player) {
		Player->FinishLevel(NextLevel);
	}
}
