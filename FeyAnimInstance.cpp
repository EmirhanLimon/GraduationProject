
#include "FeyAnimInstance.h"
#include "Fey.h"


void UFeyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Fey == nullptr)
	{
		Fey = Cast<AFey>(TryGetPawnOwner());
	}
	if(Fey)
	{
		FVector Velocity { Fey->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}