

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorCharacter.h"
#include "EarthSlam.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AEarthSlam : public AActor
{
	GENERATED_BODY()
	
public:	

	AEarthSlam();

protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void StartCameraShake();
	UFUNCTION()
	void EarthSlamOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	

	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* EarthSlamBoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShakeEarthSlam;
};
