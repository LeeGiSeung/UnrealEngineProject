// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"

void UTogetherRunAnimInstance::SetfTogetherAnimGroundSpeed(float value)
{
	fGroundSpeed = value;
}

void UTogetherRunAnimInstance::SetfTogetherAnimShouldMove(bool value)
{
	bShouldMove = value;
}
