

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorCharacter.h"
#include "GroundSmash.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AGroundSmash : public AActor
{
	GENERATED_BODY()
	
public:	

	AGroundSmash();

protected:
	void PlayCameraShake();
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void GroundSmashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* GroundSmashComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* HandImpactParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> CameraShakeGroundSmash;
};
