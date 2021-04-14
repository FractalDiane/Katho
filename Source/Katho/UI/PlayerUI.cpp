// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"

#include <Animation/WidgetAnimation.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>

void UPlayerUI::SetTimeSigilPosition(float Percent, bool TimeControl)
{
	float Pos = SigilLeftBound + SigilBarLength * Percent;
	Cast<UCanvasPanelSlot>(TimeSigil->Slot)->SetPosition(FVector2D(Pos, 0));
	TimeSigil->SetRenderTransformAngle(SigilTotalRotation * Percent);

	/*if (TimeControl) {
		if (SoundShouldBePlayed(SigilLeftBound, SigilBarLength, 200.f, PreviousSigilPosition, Pos)) {
			PlaySound(SoundSigilTick);
		}
	}*/

	PreviousSigilPosition = Pos;
}


void UPlayerUI::PlayTimeDisplayFadeAnimation(bool Fadeout)
{
	if (Fadeout) {
		PlayAnimationReverse(AnimTimeDisplayFade);
	} else {
		PlayAnimation(AnimTimeDisplayFade);
	}
}


void UPlayerUI::PlaySigilAnimation(bool Reverse)
{
	if (Reverse) {
		PlayAnimationReverse(AnimSigilSize);
	} else {
		PlayAnimation(AnimSigilSize);
	}
}


bool UPlayerUI::SoundShouldBePlayed(float A, float B, float C, float PreviousPos, float CurrentPos)
{
	float Step = C / (B - A);

	float PreviousPercent = (PreviousPos - A) / (B - A);
	float CurrentPercent = (CurrentPos - A) / (B - A);

	int32 PreviousSegment = FMath::Floor(PreviousPercent / Step);
	int32 CurrentSegment = FMath::Floor(CurrentPercent / Step);

	return PreviousSegment != CurrentSegment;
}
