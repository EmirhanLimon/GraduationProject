

#pragma once

#include "CoreMinimal.h"
#include "WarriorCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MagicBall.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	

	AMagicBall();

protected:

	virtual void BeginPlay() override;
	void Attack();
	virtual void Destroyed() override;;
	UFUNCTION()
	void AttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	

	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USphereComponent* MagicBallSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MagicBallParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Alpha;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ClampValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* WarriorHitReacts;
};
