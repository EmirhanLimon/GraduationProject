

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Rampage.h"
#include "RampageAnimInstance.generated.h"


UCLASS()
class GRADUATIONPROJECT_API URampageAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class ARampage* Rampage;
};
