// Fill out your copyright notice in the Description page of Project Settings.


#include "NarbashAnimInstance.h"

void UNarbashAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Narbash == nullptr)
	{
		Narbash = Cast<ANarbash>(TryGetPawnOwner());
	}
	if(Narbash)
	{
		FVector Velocity { Narbash->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}