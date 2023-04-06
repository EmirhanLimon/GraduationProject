

#pragma once

#include "CoreMinimal.h"
#include "WarriorCharacter.h"
#include "Components/BoxComponent.h"
#include "Fey.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AArrow();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
private:
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ArrowMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ArrowBoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ArrowCollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ArrowDestroyParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* FireArrowParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* UltimateArrowParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ClampValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Alpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GruxHitReacts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KhaimeraHitReacts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FeyHitReacts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> deneme;
};
