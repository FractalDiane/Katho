// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class KATHO_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* TimeSigil;
	UPROPERTY(meta = (BindWidget))
	class UImage* Transition;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimSigilSize;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimTimeDisplayFade;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimTransition;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SoundSigilStart;
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SoundSigilTick;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TransitionMaterialRef;
	class UMaterialInstanceDynamic* TransitionMaterial;

	static constexpr float SigilLeftBound = -600;
	static constexpr float SigilBarLength = SigilLeftBound * -2;
	static constexpr float SigilTotalRotation = 360 * 2;

	float PreviousSigilPosition = -600.f;

	FWidgetAnimationDynamicEvent EventTransition;
	FName TargetNewLevel;

public:
	virtual void NativeConstruct() override;

	void PlayTimeDisplayFadeAnimation(bool Fadeout);
	void SetTimeSigilPosition(float Percent, bool TimeControl);
	void PlaySigilAnimation(bool Reverse);
	void PlayTransitionAnimation(bool Reverse, bool BindEnd);

	inline void SetTargetNewLevel(const FName& Value) { TargetNewLevel = Value; }

private:
	bool SoundShouldBePlayed(float A, float B, float C, float PreviousPos, float CurrentPos);

	UFUNCTION()
	void ChangeLevel();
};
