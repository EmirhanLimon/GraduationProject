


#include "WarriorCharacter.h"
#include "Arrow.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "Grux.h"
#include "Khaimera.h"
#include "Narbash.h"
#include "MyPlayerCameraManager.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"


AWarriorCharacter::AWarriorCharacter() :
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	bSpeedBoost(false),
	SpeedBoostValue(100.f),
	SpeedBoostCooldown(0.f),
	SpeedBoostCooldownControl(true),
	RollClick(0.f),
	Rolling(false),
	WarriorBasicAttackClick(false),
	VelocityInterpSpeed(1.0f),
	RollReset(false),
	CharacterState(ECharacterState::ECS_Warrior),
	ChangeCharacterFormValue(100.f),
	ChangeCharacterFormTime(0.f),
	ArrowAttackHoldDownValue(0.f),
	ArcherBasicAttackReset(false),
	WarriorBasicAttackReset(false),
	ArcherCanFire(false),
	CharacterChanging(false),
	ArrowSpeed(0.f),
	CombatState(ECombatState::ECS_Unoccupied),
	BasicAttackState(EBasicAttackState::EBAS_Unoccupied),
	ThunderStormSpawned(false),
	TurnSkillUsing(false),
	CharacterHealth(100.f),
	CharacterMana(100.f),
	RainOfArrowUsing(false),
	ReinforcedArrowUsing(false),
	SwitchCounter(0),
	Alpha(0.f),
	ClampValue(0.f),
	DistanceTargetEnemy(0.f),
	GruxRendered(false),
	KhaimeraRendered(false),
	NarbashRendered(false),
	RampageRendered(false),
	FastArrowFire(false),
	ArrowSpawnedAmount(0),
	WarriorFirstSkillCooldown(0.f),
	WarriorSecondSkillCooldown(0.f),
	WarriorThirdSkillCooldown(0.f),
	WarriorFourthSkillCooldown(0.f),
	ArcherFirstSkillCooldown(0.f),
	ArcherSecondSkillCooldown(0.f),
	ArcherThirdSkillCooldown(0.f),
	ArcherFourthSkillCooldown(0.f),
	SwordAttacking(false),
	Invincibility(false),
	FeyRendered(false),
	HealthPotionAmount(0.f),
	ManaPotionAmount(0.f),
	HealthPotionCooldown(0.f),
	ManaPotionCooldown(0.f),
	ManaPotionTriggerAmount(0),
	HealthPotionTriggerAmount(0),
	InSafeZone(false),
	AmountOfDeadEnemies(5),
	EnemySpawning(false),
	WaveState(EWaveState::EWS_WaveOne),
	ArrowGettingReady(false)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	TurnFloorCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TurnFloorCollisionSphere"));
	TurnFloorCollisionSphere->SetupAttachment(GetRootComponent());

	TurnCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TurnCollisionSphere"));
	TurnCollisionSphere->SetupAttachment(TurnFloorCollisionSphere);
	
	TurnCollisionSphere2 = CreateDefaultSubobject<USphereComponent>(TEXT("TurnCollisionSphere2"));
	TurnCollisionSphere2->SetupAttachment(TurnFloorCollisionSphere);

	TurnParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TurnParticleSystemComponent"));
	TurnParticleSystemComponent->SetupAttachment(TurnFloorCollisionSphere);
	TurnParticleSystemComponent->Activate(false);

	WarriorWeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollisionCapsule"));
	WarriorWeaponCollision->SetupAttachment(GetMesh(), FName("sword_top"));

	BehindCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BehindCollision"));
	BehindCollision->SetupAttachment(GetRootComponent());

	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

AArrow* AWarriorCharacter::GetPickUpItem(AArrow* Item)
{
	auto Weapon = Cast<AArrow>(Item);
	if(Weapon)
	{
		
	}
	return Weapon;
}

void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpeedBoostCooldownTimerFunction();
	
	CameraManager = Cast<AMyPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	WarriorWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWarriorCharacter::WarriorWeaponOverlap);
	TurnCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWarriorCharacter::WarriorFirstSpinOverlap);
	TurnCollisionSphere2->OnComponentBeginOverlap.AddDynamic(this, &AWarriorCharacter::WarriorSecondSpinOverlap);
	BehindCollision->OnComponentBeginOverlap.AddDynamic(this, &AWarriorCharacter::BehindOverlap);
	BehindCollision->OnComponentEndOverlap.AddDynamic(this, &AWarriorCharacter::BehindEnd);
	ManaRestore();
	HealthRestore();
	ChangeCharacterTimer();
	RestoreCharacterChangeValue();
}

void AWarriorCharacter::MoveForward(float Value) {
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsInAir && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging) {
		
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AWarriorCharacter::MoveRight(float Value) {

	if ((Controller != nullptr) && (Value != 0.0f) && !bIsInAir  && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging) {

		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AWarriorCharacter::TurnAtRate(float Rate) {

	if(CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
	
}

void AWarriorCharacter::LookUpAtRate(float Rate) {

	if(CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
	
}

void AWarriorCharacter::AddControllerYawInput(float Value)
{
	if(CombatState == ECombatState::ECS_Unoccupied && CharacterState == ECharacterState::ECS_Warrior  && !Rolling)
	{
		APawn::AddControllerYawInput(Value);
	}
	if(CharacterState == ECharacterState::ECS_Archer  && !Rolling)
	{
		APawn::AddControllerYawInput(Value);
	}
	
}


void AWarriorCharacter::AddControllerPitchInput(float Value)
{
	if(CombatState == ECombatState::ECS_Unoccupied && CharacterState == ECharacterState::ECS_Warrior && !Rolling)
	{
		APawn::AddControllerPitchInput(Value);
	}
	if(CharacterState == ECharacterState::ECS_Archer  && !Rolling)
	{
		APawn::AddControllerPitchInput(Value);
	}
	
}

void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayRollMontage(DeltaTime);
	PawnNoiseEmitterComponent->MakeNoise(this,1.0f,GetActorLocation());
	if(CharacterState == ECharacterState::ECS_Warrior)
	{
		/*if((BasicAttackState == EBasicAttackState::EBAS_FirstAttack || BasicAttackState == EBasicAttackState::EBAS_SecondAttack || BasicAttackState == EBasicAttackState::EBAS_ThirdAttack) && (DistanceTargetEnemy < 350) && (TargetEnemy))
        	{
        		ClampValue =  FMath::Clamp((Alpha + 0.03f),0,1);
        		Alpha = ClampValue;
        		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),EnemyTargetLocation);
        		FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
        		GetController()->SetControlRotation(NewRotation);
        	}*/
	}
	else
	{
		if((FastArrowFire || RainOfArrowUsing) && (DistanceTargetEnemy < 2500))
		{
			ClampValue =  FMath::Clamp((Alpha + 0.03f),0,1);
			Alpha = ClampValue;
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),EnemyTargetLocation);
			FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
			GetController()->SetControlRotation(NewRotation);
		}
	}
	Spin(DeltaTime);
}

void AWarriorCharacter::SpeedBoost(float Value) {
	if (Value > 0.f && SpeedBoostValue > 0 && SpeedBoostCooldown <= 0 && !SpeedBoostCooldownControl && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging) {
		bSpeedBoost = true;
		SpeedBoostValue -= Value * 0.1f;
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		bSpeedBoost = false;
		if (SpeedBoostValue < 100) {
			SpeedBoostValue += 0.1f;
			if (!SpeedBoostCooldownControl && SpeedBoostValue <= 1) {
				SpeedBoostCooldown = 10.f;
				SpeedBoostCooldownControl = true;
				
			}			
		}		
	}
}

void AWarriorCharacter::Spin(float DeltaTime)
{
	if(TurnSkillUsing)
	{
		FRotator Rotation(TurnFloorCollisionSphere->GetComponentRotation().Roll,TurnFloorCollisionSphere->GetComponentRotation().Pitch,TurnFloorCollisionSphere->GetComponentRotation().Yaw + (0.001 *DeltaTime));
		TurnFloorCollisionSphere->SetWorldRotation(Rotation);
	}
}


void AWarriorCharacter::SpeedBoostCooldownTimerFunction()
{
	if (SpeedBoostCooldown >= 1 && SpeedBoostCooldownControl) {
		SpeedBoostCooldown -= 1.f;
	}
	else {
		SpeedBoostCooldownControl = false;
	}
	GetWorldTimerManager().SetTimer(SpeedBoostCooldownTimer, this, &AWarriorCharacter::SpeedBoostCooldownTimerFunction, 1.f);
}

void AWarriorCharacter::Roll()
{
	if(!CharacterChanging && !Rolling &&  CombatState == ECombatState::ECS_Unoccupied)
	{
		RollClick++;
		if(RollClick == 1  && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging)
		{
			GetWorldTimerManager().SetTimer(ClickTimer, this, &AWarriorCharacter::ClickResetFunction, 0.2f);
		}
	}
	
	
}

void AWarriorCharacter::ArcherBasicAttack(float Value)
{
	if(CharacterState == ECharacterState::ECS_Archer && !bIsInAir && !RainOfArrowUsing &&!ArcherBasicAttackReset && !CharacterChanging && (SwitchCounter == 0 || SwitchCounter == 1))
	{
		const USkeletalMeshSocket* ArrowSpawnSocket = GetMesh()->GetSocketByName(FName("ArrowSpawnSocket"));
		if(ArrowSpawnSocket && GetWorld())
		{
			const FTransform SocketTransform = ArrowSpawnSocket->GetSocketTransform(GetMesh());
			FVector ArrowSpawnLocation = SocketTransform.GetLocation();
			if (CameraManager )
			{
				const FVector CrosshairWorldLocation = CameraManager->GetCameraLocation();
				const FVector Range{15000,15000,15000};
				FVector ImpactPoint = (CameraManager->GetActorForwardVector() * Range) + CameraManager->GetCameraLocation();
				if(CharacterState == ECharacterState::ECS_Archer && !bIsInAir && !ArcherBasicAttackReset && Value >= 1)
				{
					ArcherCanFire = true;
					if(ArrowSpeed <= 99)
					{
						ArrowSpeed +=1.5f;
						CombatState = ECombatState::ECS_FireTimerInProgress;
						ArrowGettingReady = true;
					}
				}
				if(CharacterState == ECharacterState::ECS_Archer && !bIsInAir && !ArcherBasicAttackReset && Value < 1  && ArcherCanFire)
				{
					FHitResult ArrowTraceHit;
					GetWorld()->LineTraceSingleByChannel(ArrowTraceHit, CrosshairWorldLocation, ImpactPoint, ECollisionChannel::ECC_Visibility);
					if(ArrowTraceHit.bBlockingHit)
					{
						PlayAttackMontage();
						ImpactPoint = ArrowTraceHit.Location;
						FRotator ArrowSpanwRotation = (ImpactPoint - ArrowSpawnLocation).Rotation();
						GetWorld()->SpawnActor<AActor>(ActorToSpawn, ArrowSpawnLocation, ArrowSpanwRotation);
						ArrowSpeed = 0;
						ArcherCanFire = false;
						ArrowGettingReady = false;
					}
				}
			}
		}
	}
}

void AWarriorCharacter::WarriorBasicAttack()
{
	WarriorBasicAttackClick = true;
	if(CharacterState == ECharacterState::ECS_Warrior && !bIsInAir && BasicAttackState == EBasicAttackState::EBAS_Unoccupied &&
		!CharacterChanging && CombatState == ECombatState::ECS_Unoccupied && !Rolling)
	{
		PlayAttackMontage();
	}
}

void AWarriorCharacter::Jump()
{
	if(CombatState == ECombatState::ECS_Unoccupied && !RollReset && !CharacterChanging)
	{
		ACharacter::Jump();	
	}
	
}

void AWarriorCharacter::WarriorWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	ANarbash* Narbash = Cast<ANarbash>(OtherActor);
	if(Grux && SwordAttacking && !Grux->GetDied())
	{
		const float HitReactChance = FMath::FRandRange(0.f, 1.f);
		UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
		if(Grux->GetGruxCombatState() == EGruxCombatState::EGCS_Unoccupied)
		{
			if(BasicAttackState == EBasicAttackState::EBAS_FirstAttack && HitReactChance > 0.3f)
			{
				AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Grux->SetGruxCombatState(EGruxCombatState::EGCS_FireTimerInProgress);
			}
			else if(BasicAttackState == EBasicAttackState::EBAS_SecondAttack && HitReactChance > 0.2f)
			{
				AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Right"));
				Grux->SetGruxCombatState(EGruxCombatState::EGCS_FireTimerInProgress);
			}
			else if(BasicAttackState == EBasicAttackState::EBAS_ThirdAttack && HitReactChance > 0.1f)
			{
				AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Left"));
				Grux->SetGruxCombatState(EGruxCombatState::EGCS_FireTimerInProgress);
			}
		}
		Grux->SetGruxHealth(Grux->GetGruxHealth() - 20.f);
		SwordAttacking = false;
	}
	if(Khaimera && SwordAttacking && !Khaimera->GetDied())
	{
		const float HitReactChance = FMath::FRandRange(0.f, 1.f);
		UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
		if(Khaimera->GetKhaimeraCombatState() == EKhaimeraCombatState::EKCS_Unoccupied)
		{
			if(BasicAttackState == EBasicAttackState::EBAS_FirstAttack && HitReactChance > 0.3f)
			{
				AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Khaimera->SetKhaimeraCombatState(EKhaimeraCombatState::EKCS_FireTimerInProgress);
			}
			else if(BasicAttackState == EBasicAttackState::EBAS_SecondAttack && HitReactChance > 0.2f)
			{
				AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Right"));
				Khaimera->SetKhaimeraCombatState(EKhaimeraCombatState::EKCS_FireTimerInProgress);
			}
			else if(BasicAttackState == EBasicAttackState::EBAS_ThirdAttack && HitReactChance > 0.1f)
			{
				AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Left"));
				Khaimera->SetKhaimeraCombatState(EKhaimeraCombatState::EKCS_FireTimerInProgress);
			}
		}
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 20.f);
		SwordAttacking = false;
	}
	if(Narbash && SwordAttacking && !Narbash->GetDied() && !Narbash->GetInvincibility())
	{
		const float HitReactChance = FMath::FRandRange(0.f, 1.f);
		UAnimInstance* AnimInstance = Narbash->GetMesh()->GetAnimInstance();
		if(Narbash->GetNarbashCombatState() == ENarbashCombatState::ENCS_Unoccupied)
		{
			
			if(BasicAttackState == EBasicAttackState::EBAS_FirstAttack && HitReactChance > 0.3f)
			{
				AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Narbash->SetNarbashCombatState(ENarbashCombatState::ENCS_FireTimerInProgress);
			}
			else if(BasicAttackState == EBasicAttackState::EBAS_SecondAttack && HitReactChance > 0.2f)
			{
				AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
				AnimInstance->Montage_JumpToSection(FName("Right"));
				Narbash->SetNarbashCombatState(ENarbashCombatState::ENCS_FireTimerInProgress);
			}
			else if(BasicAttackState == EBasicAttackState::EBAS_ThirdAttack && HitReactChance > 0.1f)
			{
				AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
				AnimInstance->Montage_JumpToSection(FName("Left"));
				Narbash->SetNarbashCombatState(ENarbashCombatState::ENCS_FireTimerInProgress);
			}
		}
		Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 20.f);
		SwordAttacking = false;
	}
}

void AWarriorCharacter::WarriorFirstSpinOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	ANarbash* Narbash = Cast<ANarbash>(OtherActor);
	if(Grux && TurnSkillUsing)
	{
		Grux->SetGruxHealth(Grux->GetGruxHealth() - 0.050f);
	}
	if(Khaimera && TurnSkillUsing)
	{
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 0.050f);
	}
	if(Narbash && TurnSkillUsing && !Narbash->GetInvincibility())
	{
		Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 0.050f);
	}
}

void AWarriorCharacter::WarriorSecondSpinOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	ANarbash* Narbash = Cast<ANarbash>(OtherActor);
	if(Grux  && TurnSkillUsing)
	{
		Grux->SetGruxHealth(Grux->GetGruxHealth() - 0.050f);
	}
	if(Khaimera  && TurnSkillUsing)
	{
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 0.050f);
	}
	if(Narbash  && TurnSkillUsing && !Narbash->GetInvincibility())
	{
		Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 0.050f);
	}
}

void AWarriorCharacter::BehindOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	if(Grux)
	{
		Grux->SetBehind(true);
	}
	if(Khaimera)
	{
		Khaimera->SetBehind(true);
	}
}

void AWarriorCharacter::BehindEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	if(Grux)
	{
		Grux->SetBehind(false);
	}
	if(Khaimera)
	{
		Khaimera->SetBehind(false);
	}
}

void AWarriorCharacter::PlayRollMontage(float DeltaTime)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && TumbleMontage && !bIsInAir && RollClick == 2 && !RollReset && !CharacterChanging)
	{
		Rolling = true;
		if(CharacterState == ECharacterState::ECS_Warrior && CombatState == ECombatState::ECS_Unoccupied)
		{
			AnimInstance->Montage_Play(TumbleMontage);
			AnimInstance->Montage_JumpToSection(FName("Roll"));
		}
		if(CharacterState == ECharacterState::ECS_Archer && CombatState == ECombatState::ECS_Unoccupied)
		{
			AnimInstance->Montage_Play(ArcherRollMontage);
			AnimInstance->Montage_JumpToSection(FName("Roll"));
		}
		if(CombatState == ECombatState::ECS_Unoccupied)
		{
			RollClick = 0;
			GetWorldTimerManager().ClearTimer(ClickTimer);
			TargetVelocity = (GetActorForwardVector() * 1500.0f);
			RollReset = true;
			GetWorldTimerManager().SetTimer(RollResetTimer, this, &AWarriorCharacter::RollResetFunction, 2.f);
		}
		
	}
	if(Rolling)
	{
		Velocity =  GetCharacterMovement()->Velocity;
		Velocity = FMath::VInterpTo(Velocity,TargetVelocity , DeltaTime, 15.f);
		GetCharacterMovement()->Velocity = FMath::Lerp(GetCharacterMovement()->Velocity,TargetVelocity,2.f);
	}
}

void AWarriorCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && !bIsInAir && !ArcherBasicAttackReset && !WarriorBasicAttackReset)
	{
		if(CharacterState == ECharacterState::ECS_Archer)
		{
			AnimInstance->Montage_Play(ArcherAttackMontage);
			AnimInstance->Montage_JumpToSection(FName("AttackArcher"));
		}
		if(CharacterState == ECharacterState::ECS_Warrior)
		{
			AnimInstance->Montage_Play(WarriorAttackMontage);
			AnimInstance->Montage_JumpToSection(FName("FirstBasicAttack"));
			CombatState = ECombatState::ECS_FireTimerInProgress;
			WarriorBasicAttackClick = false;
		}
	}
}

void AWarriorCharacter::ChangeCharacterFunction()
{
	ChangeCharacterFormTime = 30.f;
	ChangeCharacterFormValue = 0.f;
	GetWorldTimerManager().ClearTimer(FirstSkillCooldownTimer);
	GetWorldTimerManager().ClearTimer(SecondSkillCooldownTimer);
	GetWorldTimerManager().ClearTimer(ThirdSkillCooldownTimer);
	GetWorldTimerManager().ClearTimer(FourthSkillCooldownTimer);
}

void AWarriorCharacter::ManaRestore()
{
	if(CharacterMana < 100)
	{
		CharacterMana++;
		
	}
	else
	{
		CharacterMana = 100;
	}
	GetWorldTimerManager().SetTimer(ManaRestoreTimer, this, &AWarriorCharacter::ManaRestore, 1.f);
}

void AWarriorCharacter::HealthRestore()
{
	if(CharacterHealth < 100)
	{
		CharacterHealth = CharacterHealth + 0.1f;
	}
	else
	{
		CharacterHealth = 100;
	}
	GetWorldTimerManager().SetTimer(HealthRestoreTimer, this, &AWarriorCharacter::HealthRestore, 1.f);
}

void AWarriorCharacter::UseHealthPotion()
{
	if(HealthPotionAmount >= 1 && HealthPotionCooldown <= 0)
	{
		HealthPotionCooldown = 30.f;
		HealthPotionAmount -= 1.f;
		UsingHealthPotion();
		HealthPotionTimeReset();
	}
}

void AWarriorCharacter::UseManaPotion()
{
	if(ManaPotionAmount >= 1 && ManaPotionCooldown <= 0)
	{
		ManaPotionCooldown = 30.f;
		ManaPotionAmount -= 1.f;
		UsingManaPotion();
		ManaPotionTimeReset();
	}
}

void AWarriorCharacter::UsingHealthPotion()
{
	if(CharacterHealth < 100)
	{
		CharacterHealth += 2;
		if(CharacterHealth >= 100)
		{
			CharacterHealth = 100;
		}
	}
	else
	{
		CharacterHealth = 100;
	}
	if(HealthPotionTriggerAmount <= 5)
	{
		FTimerHandle HealthPotionTriggerTimer;
		GetWorldTimerManager().SetTimer(HealthPotionTriggerTimer, this, &AWarriorCharacter::UsingHealthPotion, 1.f);
		HealthPotionTriggerAmount++;
	}
	else
	{
		HealthPotionTriggerAmount = 0;
	}

}

void AWarriorCharacter::UsingManaPotion()
{
	if(CharacterMana < 100)
	{
		CharacterMana += 2;
		if(CharacterMana >= 100)
		{
			CharacterMana = 100;
		}
	}
	else
	{
		CharacterMana = 100;
	}
	if(ManaPotionTriggerAmount <= 5)
	{
		FTimerHandle ManaPotionTriggerTimer;
		GetWorldTimerManager().SetTimer(ManaPotionTriggerTimer, this, &AWarriorCharacter::UsingManaPotion, 1.f);
		ManaPotionTriggerAmount++;
	}
	else
	{
		ManaPotionTriggerAmount = 0;
	}
}

void AWarriorCharacter::HealthPotionTimeReset()
{
	if(HealthPotionCooldown > 0)
	{
		HealthPotionCooldown--;
		FTimerHandle HealthPotionTimer;
		GetWorldTimerManager().SetTimer(HealthPotionTimer,this,&AWarriorCharacter::HealthPotionTimeReset,1.f);
	}
	else
	{
		HealthPotionCooldown = 0;
	}
}

void AWarriorCharacter::ManaPotionTimeReset()
{
	if(ManaPotionCooldown > 0)
	{
		ManaPotionCooldown--;
		FTimerHandle ManaPotionTimer;
		GetWorldTimerManager().SetTimer(ManaPotionTimer,this,&AWarriorCharacter::ManaPotionTimeReset,1.f);
	}
	else
	{
		ManaPotionCooldown = 0;
	}
}

void AWarriorCharacter::RestoreCharacterChangeValue()
{
	if(ChangeCharacterFormValue < 100)
	{
		ChangeCharacterFormValue++;
	}
	FTimerHandle RestoreTrigger;
	GetWorldTimerManager().SetTimer(RestoreTrigger,this,&AWarriorCharacter::RestoreCharacterChangeValue,1.f);
}

void AWarriorCharacter::ChangeCharacterTimer()
{
	if(ChangeCharacterFormTime >= 1)
	{
		ChangeCharacterFormTime--;
	}
	else
	{
		ChangeCharacterFormTime = 0;
	}
	FTimerHandle TimerTrigger;
	GetWorldTimerManager().SetTimer(TimerTrigger,this,&AWarriorCharacter::ChangeCharacterTimer,1.f);
}

void AWarriorCharacter::ClickResetFunction()
{
	RollClick = 0;
}

void AWarriorCharacter::RollResetFunction()
{
	RollReset = false;
}

void AWarriorCharacter::FirstSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && !bIsInAir && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging && !Rolling)
	{
		if(CharacterState == ECharacterState::ECS_Warrior && WarriorFirstSkillCooldown <= 0 && CharacterMana >= 5.f)
		{
			AnimInstance->Montage_Play(WarriorCharacterSkillSet);
			AnimInstance->Montage_JumpToSection(FName("FirstSkill"));
			CombatState = ECombatState::ECS_FireTimerInProgress;
			if(WarriorFirstFXParticle)
			{
				Invincibility = true;
				CharacterMana -= 20.f;
				WarriorFirstSkillCooldown = 20.f;
				FVector SpawnLocation(GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z + 50.f);
				WarriorFirstFXParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WarriorFirstFXParticle,SpawnLocation + (GetActorForwardVector() * 50.f) ,GetActorRotation());
				FTimerHandle DestroyFirstSkillTimer;
				GetWorldTimerManager().SetTimer(DestroyFirstSkillTimer, this, &AWarriorCharacter::DestroyFirstSkillFX, 0.90f);
				FirstSkillCooldownReset();
			}
		}
		if(CharacterState == ECharacterState::ECS_Archer && ArcherFirstSkillCooldown <= 0 && CharacterMana >= 15.f)
		{
			CharacterMana -= 15.f;
			ArcherFirstSkillCooldown = 8.f;
			SwitchCounter = 1;
			ReinforcedArrowUsing = true;
			FirstSkillCooldownReset();
		}
	}
}

void AWarriorCharacter::SecondSkill()
{
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && !bIsInAir && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging  && !Rolling)
	{
		if(CharacterState == ECharacterState::ECS_Warrior && WarriorSecondSkillCooldown <= 0 && CharacterMana >= 20.f)
		{
			CombatState = ECombatState::ECS_FireTimerInProgress;
			CharacterMana -= 20.f;
			WarriorSecondSkillCooldown = 8.f;
			AnimInstance->Montage_Play(WarriorCharacterSkillSet);
			AnimInstance->Montage_JumpToSection(FName("SecondSkill"));
			FTimerHandle SpawnSecondSkillTimer;
			GetWorldTimerManager().SetTimer(SpawnSecondSkillTimer, this, &AWarriorCharacter::PlayWarriorSecondFX, 0.55f);
			SecondSkillCooldownReset();
		}
		if(CharacterState == ECharacterState::ECS_Archer && DistanceTargetEnemy < 2500 && ArcherSecondSkillCooldown <= 0 && CharacterMana >= 20.f && (GruxRendered || FeyRendered || KhaimeraRendered || NarbashRendered ||	RampageRendered))
		{
			CharacterMana -= 20.f;
			ArcherSecondSkillCooldown = 9.f;
			AnimInstance->Montage_Play(ArcherSkillSet);
			AnimInstance->Montage_JumpToSection(FName("SecondSkill"));
			CombatState = ECombatState::ECS_FireTimerInProgress;
			FVector	ArrowSpawnLocation(EnemyTargetLocation.X,EnemyTargetLocation.Y,(EnemyTargetLocation.Z+500.f));
			FRotator ArrowSpawnRotation(270, GetActorRotation().Pitch,GetActorRotation().Yaw);
			GetWorld()->SpawnActor<AActor>(ActorToSpawn, ArrowSpawnLocation, ArrowSpawnRotation);
			SwitchCounter = 2;
			SecondSkillCooldownReset();
			RainOfArrowUsing = true;
		}
	}
}

void AWarriorCharacter::ThirdSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && !bIsInAir && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging  && !Rolling)
	{
		if(CharacterState == ECharacterState::ECS_Warrior && WarriorThirdSkillCooldown <= 0 && CharacterMana >= 25.f)
		{
			CharacterMana -= 25.f;
			WarriorThirdSkillCooldown = 14.f;
			AnimInstance->Montage_Play(WarriorCharacterSkillSet);
			AnimInstance->Montage_JumpToSection(FName("ThirdSkill"));
			GetWorldTimerManager().SetTimer(WarriorThirdSkillFXTimer, this, &AWarriorCharacter::PlayWarriorThirdFX, 0.95f);
			CombatState = ECombatState::ECS_FireTimerInProgress;
			ThirdSkillCooldownReset();
		}
		if(CharacterState == ECharacterState::ECS_Archer && DistanceTargetEnemy < 2500 && ArcherThirdSkillCooldown <= 0 && CharacterMana >= 25.f  && (GruxRendered || FeyRendered || KhaimeraRendered || NarbashRendered ||	RampageRendered))
		{
			CharacterMana -= 25.f;
			ArcherThirdSkillCooldown = 11.f;
			AnimInstance->Montage_Play(ArcherSkillSet);
			AnimInstance->Montage_JumpToSection(FName("ThirdSkill"));
			CombatState = ECombatState::ECS_FireTimerInProgress;
			SwitchCounter = 3;
			FastArrowFire = true;
			FTimerHandle FirstArrowSpawn;
			GetWorldTimerManager().SetTimer(FirstArrowSpawn, this, &AWarriorCharacter::FirstArrow, 0.10f);
			ThirdSkillCooldownReset();
		}
	}
}

void AWarriorCharacter::FirstArrow()
{
	const USkeletalMeshSocket* ArrowSpawnSocket = GetMesh()->GetSocketByName(FName("ArrowSpawnSocket"));
	const FTransform SocketTransform = ArrowSpawnSocket->GetSocketTransform(GetMesh());
	FVector ArrowSpawnLocation = SocketTransform.GetLocation();
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, ArrowSpawnLocation, GetActorRotation());
	FTimerHandle SecondAndThirdArrowSpawn;
	GetWorldTimerManager().SetTimer(SecondAndThirdArrowSpawn, this, &AWarriorCharacter::SecondAndThirdArrow, 0.60f);
}

void AWarriorCharacter::SecondAndThirdArrow()
{
	FTimerHandle SecondAndThirdArrowSpawn;
	
	if(ArrowSpawnedAmount < 2)
	{
		ArrowSpawnedAmount++;
		const USkeletalMeshSocket* ArrowSpawnSocket = GetMesh()->GetSocketByName(FName("ArrowSpawnSocket"));
		const FTransform SocketTransform = ArrowSpawnSocket->GetSocketTransform(GetMesh());
		FVector ArrowSpawnLocation = SocketTransform.GetLocation();
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, ArrowSpawnLocation, GetActorRotation());
		GetWorldTimerManager().SetTimer(SecondAndThirdArrowSpawn, this, &AWarriorCharacter::SecondAndThirdArrow, 0.60f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SecondAndThirdArrowSpawn);
		ArrowSpawnedAmount = 0;
	}
	
}

void AWarriorCharacter::FourthSkill()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(CharacterState == ECharacterState::ECS_Warrior && WarriorFourthSkillCooldown <= 0 && CharacterMana >= 20.f)
	{
		if(!TurnSkillUsing)
		{
			CharacterMana -= 20.f;
			WarriorFourthSkillCooldown = 12.f;
			TurnSkillUsing = true;
			TurnParticleSystemComponent->Activate(true);
			TurnParticleSystemComponent->SetVisibility(true);
			FTimerHandle DestroyFourthSkillTimer;
			GetWorldTimerManager().SetTimer(DestroyFourthSkillTimer, this, &AWarriorCharacter::DestroyFourthSkillFX, 5.f);
			FourthSkillCooldownReset();
		}
	}
	if(CharacterState == ECharacterState::ECS_Archer && AnimInstance && !bIsInAir && CombatState == ECombatState::ECS_Unoccupied && !CharacterChanging  && !Rolling && ArcherFourthSkillCooldown <= 0 && CharacterMana >= 20.f)
	{
		CharacterMana -= 20.f;
		AnimInstance->Montage_Play(ArcherSkillSet);
		AnimInstance->Montage_JumpToSection(FName("FourthSkill"));
		CombatState = ECombatState::ECS_FireTimerInProgress;
		uint8 SpawnedAmount = 0;
		SwitchCounter = 4;
		ArcherFourthSkillCooldown = 15.f;
		for(SpawnedAmount = 0; SpawnedAmount <= 2; SpawnedAmount++)
		{
			if(SpawnedAmount == 1)
			{
				GetWorld()->SpawnActor<AActor>(MagicTrapToSpawn, GetActorLocation() + ((GetActorForwardVector() * 300.f) - GetActorRightVector() * 400.f), GetActorRotation());
			}
			else if(SpawnedAmount == 2)
			{
				GetWorld()->SpawnActor<AActor>(MagicTrapToSpawn, GetActorLocation() + ((GetActorForwardVector() * 400.f) - GetActorRightVector() * 1.f), GetActorRotation());
			}
			else
			{
				GetWorld()->SpawnActor<AActor>(MagicTrapToSpawn, GetActorLocation() + ((GetActorForwardVector() * 300.f) - GetActorRightVector() * (-400.f)), GetActorRotation());
			}
		}
		FourthSkillCooldownReset();
	}
}

void AWarriorCharacter::FirstSkillCooldownReset()
{
	if(CharacterState == ECharacterState::ECS_Warrior)
	{
		if(WarriorFirstSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(FirstSkillCooldownTimer, this, &AWarriorCharacter::FirstSkillCooldownReset, 1.f);
			WarriorFirstSkillCooldown--;
		}
	}
	else
	{
		if(ArcherFirstSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(FirstSkillCooldownTimer, this, &AWarriorCharacter::FirstSkillCooldownReset, 1.f);
			ArcherFirstSkillCooldown--;
		}
	}
}

void AWarriorCharacter::SecondSkillCooldownReset()
{
	if(CharacterState == ECharacterState::ECS_Warrior)
	{
		if(WarriorSecondSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(SecondSkillCooldownTimer, this, &AWarriorCharacter::SecondSkillCooldownReset, 1.f);
			WarriorSecondSkillCooldown--;
		}
	}
	else
	{
		if(ArcherSecondSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(SecondSkillCooldownTimer, this, &AWarriorCharacter::SecondSkillCooldownReset, 1.f);
			ArcherSecondSkillCooldown--;
		}
	}
}

void AWarriorCharacter::ThirdSkillCooldownReset()
{
	if(CharacterState == ECharacterState::ECS_Warrior)
	{
		if(WarriorThirdSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(ThirdSkillCooldownTimer, this, &AWarriorCharacter::ThirdSkillCooldownReset, 1.f);
			WarriorThirdSkillCooldown--;
		}
	}
	else
	{
		if(ArcherThirdSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(ThirdSkillCooldownTimer, this, &AWarriorCharacter::ThirdSkillCooldownReset, 1.f);
			ArcherThirdSkillCooldown--;
		}
	}
}

void AWarriorCharacter::FourthSkillCooldownReset()
{
	if(CharacterState == ECharacterState::ECS_Warrior)
	{
		if(WarriorFourthSkillCooldown >= 1)
		{
			
			GetWorldTimerManager().SetTimer(FourthSkillCooldownTimer, this, &AWarriorCharacter::FourthSkillCooldownReset, 1.f);
			WarriorFourthSkillCooldown--;
		}
	}
	else
	{
		if(ArcherFourthSkillCooldown >= 1)
		{
			GetWorldTimerManager().SetTimer(FourthSkillCooldownTimer, this, &AWarriorCharacter::FourthSkillCooldownReset, 1.f);
			ArcherFourthSkillCooldown--;
		}
	}
}

void AWarriorCharacter::PlayWarriorThirdFX()
{
	const USkeletalMeshSocket* SwordFXSocket = GetMesh()->GetSocketByName(FName("FX_Sword_Top"));
	const FTransform SocketTransform = SwordFXSocket->GetSocketTransform(GetMesh());
	FVector FXSpawnLocation = SocketTransform.GetLocation();
	FRotator FXSpawnRotation = GetActorRotation();
	if(EarthSlamParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EarthSlamParticle,FXSpawnLocation ,GetActorRotation());
		GetWorld()->SpawnActor<AActor>(EarthSlamToSpawn,FXSpawnLocation, FXSpawnRotation);
	}
	
}

void AWarriorCharacter::PlayWarriorSecondFX()
{
	FVector SpawnLocation(GetActorLocation().X ,GetActorLocation().Y,GetActorLocation().Z + 30.f);
	GetWorld()->SpawnActor<AActor>(MagicAbilityToSpawn, SpawnLocation, GetActorRotation());
	ThunderStormSpawned = true;
}

void AWarriorCharacter::DestroyFirstSkillFX()
{
	WarriorFirstFXParticleComponent->DestroyComponent();
	Invincibility = false;
}

void AWarriorCharacter::DestroyFourthSkillFX()
{
	if(TurnSkillUsing == true)
	{
		TurnParticleSystemComponent->SetVisibility(false);
		TurnSkillUsing = false;
	}
	
}


void AWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWarriorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWarriorCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWarriorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWarriorCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &AWarriorCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AWarriorCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("SpeedBoost", this, &AWarriorCharacter::SpeedBoost);
	PlayerInputComponent->BindAxis("ArcherBasicAttack", this, &AWarriorCharacter::ArcherBasicAttack);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWarriorCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AWarriorCharacter::Roll);
	PlayerInputComponent->BindAction("WarriorBasicAttack", IE_Pressed, this, &AWarriorCharacter::WarriorBasicAttack);
	PlayerInputComponent->BindAction("FirstAbility", IE_Pressed, this, &AWarriorCharacter::FirstSkill);
	PlayerInputComponent->BindAction("SecondAbility", IE_Pressed, this, &AWarriorCharacter::SecondSkill);
	PlayerInputComponent->BindAction("ThirdAbility", IE_Pressed, this, &AWarriorCharacter::ThirdSkill);
	PlayerInputComponent->BindAction("FourthAbility", IE_Pressed, this, &AWarriorCharacter::FourthSkill);
	PlayerInputComponent->BindAction("HealthPotion", IE_Pressed, this, &AWarriorCharacter::UseHealthPotion);
	PlayerInputComponent->BindAction("ManaPotion", IE_Pressed, this, &AWarriorCharacter::UseManaPotion);
}





