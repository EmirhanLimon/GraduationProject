


#include "GruxAnimInstance.h"

void UGruxAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Grux == nullptr)
	{
		Grux = Cast<AGrux>(TryGetPawnOwner());
	}
	if(Grux)
	{
		FVector Velocity { Grux->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}