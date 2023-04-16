

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "GameFramework/Character.h"
#include "WarriorCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem.h"
#include "Narbash.generated.h"

class UPawnSensingComponent;
class USphereComponent;
class AAIController;

UENUM(BlueprintType)
enum class ENarbashCombatState : uint8
{
	ENCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ENCS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UENUM(BlueprintType)
enum class ENarbashHammerState : uint8
{
	ENHS_FirstHammer UMETA(DisplayName = "FirstHammer"),
	ENHS_SecondHammer UMETA(DisplayName = "SecondHammer"),
	ENHS_ThirdHammer UMETA(DisplayName = "ThirdHammer"),
	ENHS_EndHammer UMETA(DisplayName = "EndHammer"),
};

UCLASS()
class GRADUATIONPROJECT_API ANarbash : public ACharacter
{
	GENERATED_BODY()

public:

	ANarbash();

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	void Die();
	virtual void Destroyed() override;
	UFUNCTION()
	void LeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatRangeOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void ThrowHammer();
	void ThrowHammerTriggerFunc();
	void ThrowHammerMontage();
	void SpawnFirstAbility();
	void PlayFirstSkillAnim();
	void EndInvincibility();
	void SpawnSafeZone();
public:	

	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle TriggerSafeZone;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
		AWarriorCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
		AEnemyController* AIC_Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess))
		UPawnSensingComponent* PawnSensing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USphereComponent* CombatRangeSphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bNarbashStunned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bCombatRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bCanDoDamageLeftWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bCanDoDamageRightWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool HammerThrown;
		bool FirstSkillUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float NarbashHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* LeftWeaponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RightWeaponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* NarbashAttacks;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		ENarbashCombatState NarbashCombatState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		ENarbashHammerState NarbashHammerState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bNarbashDied;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bBehind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bInvincibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* NarbashDieMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* NarbashHitReacts;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> ThrowHammertoSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> MagicCapsuletoSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> SafeZonetoSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* AuraParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		uint8 TriggerValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		uint8 TriggerSafeZoneValue;

public:
	FORCEINLINE void SetNarbashStunned(bool NewNarbashStunned) { bNarbashStunned = NewNarbashStunned; }
	FORCEINLINE bool GetNarbashStunned() const { return bNarbashStunned; }
	FORCEINLINE bool GetDied() const { return bNarbashDied; }
	FORCEINLINE bool GetInvincibility() const { return bInvincibility; }
	FORCEINLINE UAnimMontage* GetHitReactsAnimMontage() const { return NarbashHitReacts; }
	FORCEINLINE void SetNarbashHealth(float NewNarbashHealth) { NarbashHealth = NewNarbashHealth; }
	FORCEINLINE float GetNarbashHealth() const { return NarbashHealth; }
	FORCEINLINE ENarbashCombatState GetNarbashCombatState() const { return NarbashCombatState; }
	FORCEINLINE void SetNarbashCombatState(ENarbashCombatState NewNarbashCombatState) { NarbashCombatState = NewNarbashCombatState; }
	FORCEINLINE void SetNarbashDied(bool NewNarbashDied) { bNarbashDied = NewNarbashDied; }
	FORCEINLINE void SetBehind(bool NewBehind) { bBehind = NewBehind; }
	
};
