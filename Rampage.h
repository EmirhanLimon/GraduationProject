

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
	void ThrowStone();
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


public:
	FORCEINLINE void SetbIsInAir(bool NewbIsInAir) { bIsInAir = NewbIsInAir; }

};
