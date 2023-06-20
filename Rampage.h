

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "GameFramework/Character.h"
#include "WarriorCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem.h"
#include "Rampage.generated.h"

class UPawnSensingComponent;
class USphereComponent;
class AAIController;

UENUM(BlueprintType)
enum class ERampageCombatState : uint8
{
	ERCS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ERCS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UCLASS()
class GRADUATIONPROJECT_API ARampage : public ACharacter
{
	GENERATED_BODY()

public:

	ARampage();

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume);
	void CanJump();
	void ThrowStone();
	void PlayTrainingMontage();
	void PlayThrowStoneAnimMontage();
	void Jumping();
	void PlayJumpEndFX();
	void Attack();
	void GroundSmashSpawn();
	virtual void Jump() override;
	UFUNCTION()
	void LeftArmOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RightArmOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bRampageDied;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCanJump;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AEnemyController* AIC_Ref;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy", meta = (AllowPrivateAccess))
	UPawnSensingComponent* RampagePawnSensing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ERampageCombatState RampageCombatState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ThrowStoneMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* TrainingMontage;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> StonetoSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<AActor> GroundSmashtoSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ClampValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Alpha;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Distance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bJumping;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bTraining;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bLeftArmCanDamage;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bRightArmCanDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* JumpEndFxParticle;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<UCameraShakeBase> CameraShakeJumpEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftArmBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RightArmBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float RampageHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundCue* TrainingSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundCue* ThrowStoneSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundCue* AttackSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundCue* JumpSoundCue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector FirstJumpTargetLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector SecondJumpTargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	uint8 JumpAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bMovementStop;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bFirstJump;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bSecondJump;
	FTimerHandle bCanJumpTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float FirstJumpDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float SecondJumpDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bFirstJumping;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bSecondJumping;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bBehind;
public:
	FORCEINLINE float GetRampageHealth() const { return RampageHealth; }
	FORCEINLINE bool GetRampageDied() const { return bRampageDied; }
	FORCEINLINE void SetRampageHealth(float NewRampageHealth) { RampageHealth = NewRampageHealth; }
	FORCEINLINE void SetbIsInAir(bool NewbIsInAir) { bIsInAir = NewbIsInAir; }
	FORCEINLINE void SetBehind(bool NewbBehind) { bBehind = NewbBehind; }
};
