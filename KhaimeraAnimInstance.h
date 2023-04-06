

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KhaimeraAnimInstance.generated.h"


UCLASS()
class GRADUATIONPROJECT_API UKhaimeraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AKhaimera* Khaimera;
};
