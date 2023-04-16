
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NarbashCapsuleAbility.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ANarbashCapsuleAbility : public AActor
{
	GENERATED_BODY()
	
public:	

	ANarbashCapsuleAbility();

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void CapsuleAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void Destroyed() override;

public:	

	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* MagicCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* MagicCapsuleParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MagicUndulationParticleSystemComponent;
	bool Collision;


};
