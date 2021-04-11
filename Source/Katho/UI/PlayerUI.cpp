// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include <Components/Image.h>
#include <Components/CanvasPanelSlot.h>
#include <Animation/WidgetAnimation.h>
#include <Kismet/GameplayStatics.h>

void UPlayerUI::SetTimeSigilPosition(float Percent, bool TimeControl)
{
	float Pos = SigilLeftBound + SigilBarLength * Percent;
	Cast<UCanvasPanelSlot>(TimeSigil->Slot)->SetPosition(FVector2D(Pos, 0));
	TimeSigil->SetRenderTransformAngle(SigilTotalRotation * Percent);
}


void UPlayerUI::PlayTimeDisplayFadeAnimation(bool Fadeout)
{
	if (Fadeout) {
		PlayAnimationReverse(AnimTimeDisplayFade);
	} else {
		PlayAnimation(AnimTimeDisplayFade);
	}
}


void UPlayerUI::PlaySigilTickSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), SoundSigilTick, 0.3f);
}


void UPlayerUI::PlaySigilAnimation(bool Reverse)
{
	if (Reverse) {
		PlayAnimationReverse(AnimSigilSize);
	} else {
		PlayAnimation(AnimSigilSize);
	}
}
