// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "GameFramework/Character.h"
#include "WarriorCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem.h"
#include "Fey.generated.h"

class UPawnSensingComponent;
class USphereComponent;
class AAIController;

UENUM(BlueprintType)
enum class EFeyCombatState : uint8
{
	EFCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EFCS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UCLASS()
class GRADUATIONPROJECT_API AFey : public ACharacter
{
	GENERATED_BODY()

public:
	
	AFey();
	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	void CombatRange();
	void Die();
	void SpawnMagicBallLeft();
	void SpawnMagicBallRight();
	void Attack();
	virtual void Destroyed() override;
protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AEnemyController* AIC_Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess))
	UPawnSensingComponent* PawnSensing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCombatRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float FeyHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EFeyCombatState FeyCombatState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bFeyDied;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Alpha;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ClampValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FeyDieMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FeyAttackMontage;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> MagicBallToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* AttackParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FeyHitReacts;


public:
	FORCEINLINE bool GetDied() const { return bFeyDied; }
	FORCEINLINE void SetFeyHealth(float NewFeyHealth) { FeyHealth = NewFeyHealth; }
	FORCEINLINE float GetFeyHealth() const { return FeyHealth; }
	FORCEINLINE UAnimMontage* GetFeyHitReacts() const { return FeyHitReacts; }
	FORCEINLINE EFeyCombatState GetFeyCombatState() const { return FeyCombatState; }
	FORCEINLINE void SetFeyCombatState(EFeyCombatState NewFeyCombatState) { FeyCombatState = NewFeyCombatState; }
	FORCEINLINE void SetFeyDied(bool NewFeyDied) { bFeyDied = NewFeyDied; }
};
