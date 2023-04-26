

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarriorCharacter.h"
#include "RampageStone.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ARampageStone : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARampageStone();

protected:
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void RockOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void CollisionResponse();
public:	
	
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RampageStoneBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RampageStoneStaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FVector FirstTargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ClampValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Alpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool Simulate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* RockHitWorldParticle;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> CameraShakeHitStone;
	
};
