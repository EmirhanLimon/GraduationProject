

#pragma once

#include "CoreMinimal.h"
#include "WarriorCharacter.h"
#include "GameFramework/Actor.h"
#include "SafeZoneParticle.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ASafeZoneParticle : public AActor
{
	GENERATED_BODY()
	
public:	

	ASafeZoneParticle();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void SafeZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SafeZoneEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void Destroyed() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SafeZoneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SafeZoneParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;

};
