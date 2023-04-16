

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "GameFramework/Character.h"
#include "WarriorCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem.h"
#include "Khaimera.generated.h"


class UPawnSensingComponent;
class USphereComponent;
class AAIController;

UENUM(BlueprintType)
enum class EKhaimeraCombatState : uint8
{
	EKCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EKCS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UCLASS()

class GRADUATIONPROJECT_API AKhaimera : public ACharacter
{
	GENERATED_BODY()

public:

	AKhaimera();

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	void Patrol();
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
	USphereComponent* CombatRangeSphereComponent;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bKhaimeraStunned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCombatRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanDoDamageLeftWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bCanDoDamageRightWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float KhaimeraHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LeftWeaponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWeaponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KhaimeraAttacks;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EKhaimeraCombatState KhaimeraCombatState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bKhaimeraDied;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bBehind;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KhaimeraDieMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KhaimeraHitReacts;

	public:
	FORCEINLINE void SetKhaimeraStunned(bool NewKhaimeraStunned) { bKhaimeraStunned = NewKhaimeraStunned; }
	FORCEINLINE bool GetKhaimeraStunned() const { return bKhaimeraStunned; }
	FORCEINLINE bool GetDied() const { return bKhaimeraDied; }
	FORCEINLINE void SetKhaimeraHealth(float NewKhaimeraHealth) { KhaimeraHealth = NewKhaimeraHealth; }
	FORCEINLINE float GetKhaimeraHealth() const { return KhaimeraHealth; }
	FORCEINLINE UAnimMontage* GetKhaimeraHitReacts() const { return KhaimeraHitReacts; }
	FORCEINLINE EKhaimeraCombatState GetKhaimeraCombatState() const { return KhaimeraCombatState; }
	FORCEINLINE void SetKhaimeraCombatState(EKhaimeraCombatState NewKhaimeraCombatState) { KhaimeraCombatState = NewKhaimeraCombatState; }
	FORCEINLINE void SetKhaimeraDied(bool NewKhaimeraDied) { bKhaimeraDied = NewKhaimeraDied; }
	FORCEINLINE void SetBehind(bool NewBehind) { bBehind = NewBehind; }
};
