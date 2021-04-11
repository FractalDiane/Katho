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

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimSigilSize;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* AnimTimeDisplayFade;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SoundSigilStart;
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* SoundSigilTick;

	static constexpr float SigilLeftBound = -600;
	static constexpr float SigilBarLength = SigilLeftBound * -2;
	static constexpr float SigilTotalRotation = 360 * 2;

public:
	void PlayTimeDisplayFadeAnimation(bool Fadeout);
	void SetTimeSigilPosition(float Percent, bool TimeControl);
	void PlaySigilTickSound();
	void PlaySigilAnimation(bool Reverse);
};
