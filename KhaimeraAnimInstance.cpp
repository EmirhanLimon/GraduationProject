


#include "KhaimeraAnimInstance.h"

#include "Khaimera.h"

void UKhaimeraAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Khaimera == nullptr)
	{
		Khaimera = Cast<AKhaimera>(TryGetPawnOwner());
	}
	if(Khaimera)
	{
		FVector Velocity { Khaimera->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
	}
}
