// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "WarriorCharacter.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Warrior UMETA(DisplayName = "Warrior"),
	ECS_Archer UMETA(DisplayName = "Archer")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress")
};

UENUM(BlueprintType)
enum class EBasicAttackState : uint8
{
	EBAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EBAS_FirstAttack UMETA(DisplayName = "FirstAttack"),
	EBAS_SecondAttack UMETA(DisplayName = "SecondAttack"),
	EBAS_ThirdAttack UMETA(DisplayName = "ThirdAttack")
};

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	EWS_WaveOne UMETA(DisplayName = "WaveOne"),
	EWS_WaveTwo UMETA(DisplayName = "WaveTwo"),
	EWS_WaveThree UMETA(DisplayName = "WaveThree"),
	EWS_WaveFour UMETA(DisplayName = "WaveFour"),
	EWS_WaveFive UMETA(DisplayName = "WaveFive")
};
class AArrow;
UCLASS()
class GRADUATIONPROJECT_API AWarriorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AWarriorCharacter();
	AArrow* GetPickUpItem(AArrow* Item);
protected:
	
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void SpeedBoost(float Value);
	void Spin(float DeltaTime);
	virtual void AddControllerYawInput(float Value) override;
	virtual void AddControllerPitchInput(float Value) override;
	void Roll();
	void ArcherBasicAttack(float Value);
	void WarriorBasicAttack();
	virtual void Jump() override;
	UFUNCTION()
	void WarriorWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WarriorFirstSpinOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void WarriorSecondSpinOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BehindOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BehindEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BlueprintReadOnly)
	float ArrowSpeed;
	UPROPERTY(BlueprintReadOnly)
	float ArrowDamageWithSpeed;
	
private:
	UFUNCTION(BlueprintCallable)
	EPhysicalSurface GetSurfaceTypes();
	void FirstSkill();
	void SecondSkill();
	void ThirdSkill();
	void FourthSkill();
	void SpeedBoostCooldownTimerFunction();
	void ClickResetFunction();
	void RollResetFunction();
	void PlayRollMontage(float DeltaTime);
	void PlayAttackMontage();
	void PlayWarriorThirdFX();
	void PlayWarriorSecondFX();
	void DestroyFirstSkillFX();
	void DestroyFourthSkillFX();
	void FirstArrow();
	void SecondAndThirdArrow();
	void FirstSkillCooldownReset();
	void SecondSkillCooldownReset();
	void ThirdSkillCooldownReset();
	void FourthSkillCooldownReset();
	UFUNCTION(BlueprintCallable)
	void ChangeCharacterFunction();
	void ManaRestore();
	void HealthRestore();
	void UseHealthPotion();
	void UseManaPotion();
	void UsingHealthPotion();
	void UsingManaPotion();
	void HealthPotionTimeReset();
	void ManaPotionTimeReset();
	void RestoreCharacterChangeValue();
	void ChangeCharacterTimer();
	

	FTimerHandle SpeedBoostCooldownTimer;
	FTimerHandle ClickTimer;
	FTimerHandle RollResetTimer;
	FTimerHandle ChangeCharacterFormTimer;
	FTimerHandle WarriorThirdSkillFXTimer;
	FTimerHandle WarriorSecondSkillFXTimer;
	FTimerHandle FirstSkillCooldownTimer;
	FTimerHandle SecondSkillCooldownTimer;
	FTimerHandle ThirdSkillCooldownTimer;
	FTimerHandle FourthSkillCooldownTimer;
	FTimerHandle ManaRestoreTimer;
	FTimerHandle HealthRestoreTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		uint8 SwitchCounter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float AmountOfDeadEnemies;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category = Camera, meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bSpeedBoost;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float SpeedBoostValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float SpeedBoostCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float VelocityInterpSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	    float ChangeCharacterFormValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float ChangeCharacterFormTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float ArrowAttackHoldDownValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float Alpha;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ClampValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool SpeedBoostCooldownControl;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool CharacterChanging;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool RollReset;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool Rolling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool ArcherBasicAttackReset;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool ArcherCanFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool WarriorBasicAttackReset;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool WarriorBasicAttackClick;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool ThunderStormSpawned;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool TurnSkillUsing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool RainOfArrowUsing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		bool ReinforcedArrowUsing;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool GruxRendered;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool FeyRendered;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool KhaimeraRendered;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool NarbashRendered;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool RampageRendered;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool FastArrowFire;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool SwordAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool Invincibility;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool InSafeZone;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool EnemySpawning;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		bool ArrowGettingReady;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		int32 RollClick;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		FVector TargetVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* TumbleMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ArcherRollMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ArcherAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* WarriorAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* WarriorCharacterChangeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ArcherCharacterChangeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* WarriorCharacterSkillSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ArcherSkillSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* WarriorCharacterHitReacts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* ArcherCharacterHitReacts;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		ECharacterState CharacterState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		EBasicAttackState BasicAttackState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		ECombatState CombatState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		EWaveState WaveState;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> MagicAbilityToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> MagicTrapToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> EarthSlamToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
		AMyPlayerCameraManager* CameraManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* WarriorFirstFXParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* BloodFXParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* WarriorFirstFXParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* WarriorFourthFXParticleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* TurnFloorCollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USphereComponent* TurnCollisionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USphereComponent* TurnCollisionSphere2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* TargetEnemyCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* WarriorWeaponCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* TurnParticleSystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* EarthSlamParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* WarriorJumpSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* WarriorBasicAttackSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* WarriorShieldSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* WarriorThunderStormSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* WarriorEarthSlamSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* WarriorHitReactSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* ArcherHitReactSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* GruxHitReactSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* KhaimeraHitReactSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* NarbashHitReactSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* FeyHitReactSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* ArcherJumpSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		USoundCue* StoneHitWorldSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float CharacterHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float CharacterMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float DistanceTargetEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		FVector EnemyTargetLocation;
		uint8 ArrowSpawnedAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float WarriorFirstSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float WarriorSecondSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float WarriorThirdSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float WarriorFourthSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ArcherFirstSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ArcherSecondSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ArcherThirdSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ArcherFourthSkillCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UPawnNoiseEmitterComponent* PawnNoiseEmitterComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BehindCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float HealthPotionAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ManaPotionAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float HealthPotionCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		float ManaPotionCooldown;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		uint8 HealthPotionTriggerAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
		uint8 ManaPotionTriggerAmount;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UCameraShakeBase> CameraShakeHitReact;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> HealthPotiontoSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
		TSubclassOf<AActor> ManaPotiontoSpawn;
	
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE uint8 GetSwitchCounter() const { return SwitchCounter; }
	FORCEINLINE float GetAmountOfDeadEnemies() const { return AmountOfDeadEnemies; }
	FORCEINLINE FVector GetEnemyTargetLocation() const { return EnemyTargetLocation; }
	FORCEINLINE float GetCharacterHealth() const { return CharacterHealth; }
	FORCEINLINE float GetHealthPotionAmount() const { return HealthPotionAmount; }
	FORCEINLINE float GetManaPotionAmount() const { return ManaPotionAmount; }
	FORCEINLINE bool GetSpeedBoost() const { return bSpeedBoost; }
	FORCEINLINE bool GetThunderStormSpawned() const { return ThunderStormSpawned; }
	FORCEINLINE bool GetRainOfArrowUsing() const { return RainOfArrowUsing; }
	FORCEINLINE bool GetIsInAir() const { return bIsInAir; }
	FORCEINLINE bool GetRolling() const { return Rolling; }
	FORCEINLINE bool GetCharacterChanging() const { return CharacterChanging; }
	FORCEINLINE bool GetFeyRendered() const { return FeyRendered; }
	FORCEINLINE bool GetNarbashRendered() const { return NarbashRendered; }
	FORCEINLINE bool GetInvincibility() const { return Invincibility; }
	FORCEINLINE bool GetSafeZone() const { return InSafeZone; }
	FORCEINLINE bool GetEnemySpawning() const { return EnemySpawning; }
	FORCEINLINE AMyPlayerCameraManager* GetCameraManager() const { return CameraManager; }
	FORCEINLINE UAnimMontage* GetWarriorCharacterHitReacts() const { return WarriorCharacterHitReacts; }
	FORCEINLINE UAnimMontage* GetArcherCharacterHitReacts() const { return ArcherCharacterHitReacts; }
	FORCEINLINE UParticleSystem* GetBloodFXParticle() const { return BloodFXParticle; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE EWaveState GetWaveState() const { return WaveState; }
	FORCEINLINE TSubclassOf<UCameraShakeBase> GetCameraShakeHitReact() const { return CameraShakeHitReact; }
	FORCEINLINE TSubclassOf<AActor> GetHealthPotion() const { return HealthPotiontoSpawn; }
	FORCEINLINE TSubclassOf<AActor> GetManaPotion() const { return ManaPotiontoSpawn; }
	FORCEINLINE USoundCue* GetWarriorHitReactSoundCue() const { return WarriorHitReactSoundCue; }
	FORCEINLINE USoundCue* GetArcherHitReactSoundCue() const { return ArcherHitReactSoundCue; }
	FORCEINLINE USoundCue* GetGruxHitReactSoundCue() const { return GruxHitReactSoundCue; }
	FORCEINLINE USoundCue* GetKhaimeraHitReactSoundCue() const { return KhaimeraHitReactSoundCue; }
	FORCEINLINE USoundCue* GetNarbashHitReactSoundCue() const { return NarbashHitReactSoundCue; }
	FORCEINLINE USoundCue* GetFeyHitReactSoundCue() const { return FeyHitReactSoundCue; }
	FORCEINLINE USoundCue* GetStoneHitWorldSoundCue() const { return StoneHitWorldSoundCue; }
	FORCEINLINE void SetReinforcedArrowUsing(bool NewReinforcedArrowUsing) { ReinforcedArrowUsing = NewReinforcedArrowUsing; }
	FORCEINLINE void SetbIsInAir(bool NewbIsInAir) { bIsInAir = NewbIsInAir; }
	FORCEINLINE void SetAmountOfDeadEnemies(float NewAmountOfDeadEnemies) { AmountOfDeadEnemies = NewAmountOfDeadEnemies; }
	FORCEINLINE void SetGruxRendered(bool NewGruxRendered) { GruxRendered = NewGruxRendered; }
	FORCEINLINE void SetKhaimeraRendered(bool NewKhaimeraRendered) { KhaimeraRendered = NewKhaimeraRendered; }
	FORCEINLINE void SetFeyRendered(bool NewFeyRendered) { FeyRendered = NewFeyRendered; }
	FORCEINLINE void SetNarbashRendered(bool NewNarbashRendered) { NarbashRendered = NewNarbashRendered; }
	FORCEINLINE void SetRampageRendered(bool NewRampageRendered) { RampageRendered = NewRampageRendered; }
	FORCEINLINE void SetSwitchCounter(uint8 NewSwitchCounter) { SwitchCounter = NewSwitchCounter; }
	FORCEINLINE void SetCharacterHealth(float NewCharacterHealth) { CharacterHealth = NewCharacterHealth; }
	FORCEINLINE void SetCombatState(ECombatState NewCombatState) { CombatState = NewCombatState; }
	FORCEINLINE void SetHealthPotionAmount(float NewHealthPotionAmount) { HealthPotionAmount = NewHealthPotionAmount; }
	FORCEINLINE void SetManaPotionAmount(float NewManaPotionAmount) { ManaPotionAmount = NewManaPotionAmount; }
	FORCEINLINE void SetSafeZone(bool NewInSafeZone) { InSafeZone = NewInSafeZone; }
	FORCEINLINE void SetEnemySpawning(bool NewEnemySpawning) { EnemySpawning = NewEnemySpawning; }
	FORCEINLINE void SetWaveState(EWaveState NewWaveState) { WaveState = NewWaveState; }
};
