

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorCharacter.h"
#include "MagicAbility.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AMagicAbility : public AActor
{
	GENERATED_BODY()
	
public:	

	AMagicAbility();

protected:
	
	virtual void BeginPlay() override;
	void DestroyFunction();
	void StartCameraShake();
	UFUNCTION()
	void MagicAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* ThunderStormCollisionCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ThunderStormParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeThunderStorm;
public:	
	
	virtual void Tick(float DeltaTime) override;

};
