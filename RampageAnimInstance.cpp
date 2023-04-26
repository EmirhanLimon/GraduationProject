


#include "RampageAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void URampageAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(Rampage == nullptr)
	{
		Rampage = Cast<ARampage>(TryGetPawnOwner());
	}
	if(Rampage)
	{
		FVector Velocity { Rampage->GetVelocity() };
		Velocity.Z = 0.f;
		Speed = Velocity.Size();
		bIsInAir = Rampage->GetCharacterMovement()->IsFalling();
		Rampage->SetbIsInAir(bIsInAir);
	}
}
