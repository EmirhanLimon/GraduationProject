


#include "CharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime) {

	if (WarriorCharacter == nullptr) {
		WarriorCharacter = Cast<AWarriorCharacter>(TryGetPawnOwner());
	}
	if (WarriorCharacter) {
		Velocity = WarriorCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = WarriorCharacter->GetCharacterMovement()->IsFalling();
		WarriorCharacter->SetbIsInAir(bIsInAir);

		if (WarriorCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
		
		bSpeedBoostControl = WarriorCharacter->GetSpeedBoost();
		FRotator AimRotation = WarriorCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(WarriorCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
		OffsetPitch = AimRotation.Pitch;
	}
}
 void UCharacterAnimInstance::NativeInitializeAnimation()
{
	WarriorCharacter = Cast<AWarriorCharacter>(TryGetPawnOwner());
}
