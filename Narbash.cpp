
#include "Narbash.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ANarbash::ANarbash() :
	bNarbashStunned(false),
	NarbashHealth(500.f),
	bCombatRange(false),
	NarbashCombatState(ENarbashCombatState::ENCS_Unoccupied),
	bCanAttack(true),
	bCanDoDamageLeftWeapon(false),
	bCanDoDamageRightWeapon(false),
	bNarbashDied(false),
	bBehind(false),
	HammerThrown(false),
	FirstSkillUsed(false),
	TriggerValue(0),
	bInvincibility(false),
	TriggerSafeZoneValue(0),
	NarbashHammerState(ENarbashHammerState::ENHS_FirstHammer)
{

	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	//CombatRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRangeSphereComponent"));
	//CombatRangeSphereComponent->SetupAttachment(GetRootComponent());

	LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponCollision"));
	LeftWeaponCollision->SetupAttachment(GetMesh(), FName("weapon_l"));

	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponCollision"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("weapon_r"));
	
	AuraParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AuraParticleSystemComponent"));
	AuraParticleSystemComponent->SetupAttachment(GetRootComponent());

}


void ANarbash::BeginPlay()
{
	Super::BeginPlay();

	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PawnSensing->OnHearNoise.AddDynamic(this, &ANarbash::OnHearNoise);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ANarbash::LeftWeaponOverlap);
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ANarbash::RightWeaponOverlap);
	//CombatRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANarbash::CombatRangeOverlap);
	//CombatRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANarbash::CombatRangeOverlapEnd);
	AuraParticleSystemComponent->SetVisibility(false);
	
}

void ANarbash::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && NarbashCombatState == ENarbashCombatState::ENCS_Unoccupied && !bNarbashDied && !bNarbashStunned && !bInvincibility)
	{
		if(!IsValid(AIC_Ref))
		{
			AIC_Ref = Cast<AEnemyController>(GetController());
		}
		else
		{
			AIC_Ref->MoveToLocation(Character->GetActorLocation(), -1.f);
			GetCharacterMovement()->MaxWalkSpeed = 600;
			ThrowHammerMontage();
		}
	}
}


void ANarbash::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		const float RandomDie = FMath::RandRange(0.f,1.f);
		if(RandomDie <= 0.5f)
		{
			AnimInstance->Montage_Play(NarbashDieMontage);
			AnimInstance->Montage_JumpToSection(FName("DeathA"));
		}
		else
		{
			AnimInstance->Montage_Play(NarbashDieMontage);
			AnimInstance->Montage_JumpToSection(FName("DeathB"));
		}
	}
	FTimerHandle DieTimer;
	GetWorldTimerManager().SetTimer(DieTimer, this, &ANarbash::Destroyed, 1.4f);
}

void ANarbash::Destroyed()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		Character->SetAmountOfDeadEnemies(5);
		Character->SetWaveState(EWaveState::EWS_WaveFive);
	}
	Destroy();
}

void ANarbash::LeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bCanDoDamageLeftWeapon && !bNarbashDied && !bNarbashStunned && !bInvincibility)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 3);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
			if(Character->GetBloodFXParticle())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Character->GetBloodFXParticle(),Character->GetActorLocation(),FRotator(0,0,0));
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 3);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}

		bCanDoDamageLeftWeapon = false;
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(Character->GetCombatState() == ECombatState::ECS_Unoccupied && !Character->GetIsInAir() && !Character->GetRolling() && !Character->GetCharacterChanging())
		{
			if(Character->GetCameraManager())
			{
				Character->GetCameraManager()->StartCameraShake(Character->GetCameraShakeHitReact(),1);
			}
			if(Character->GetCharacterState() == ECharacterState::ECS_Warrior)
			{
				UGameplayStatics::PlaySound2D(Character,Character->GetWarriorHitReactSoundCue());
				if(bBehind)
				{
					AnimInstance->Montage_Play(Character->GetWarriorCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Back"));
				}
				else
				{
					AnimInstance->Montage_Play(Character->GetWarriorCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
				}
				Character->SetCombatState(ECombatState::ECS_FireTimerInProgress);
			}
			if(Character->GetCharacterState() == ECharacterState::ECS_Archer)
			{
				UGameplayStatics::PlaySound2D(Character,Character->GetArcherHitReactSoundCue());
				if(bBehind)
				{
					AnimInstance->Montage_Play(Character->GetArcherCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Back"));
				}
				else
				{
					AnimInstance->Montage_Play(Character->GetArcherCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
				}
				Character->SetCombatState(ECombatState::ECS_FireTimerInProgress);
			}
		}
		
	}
}

void ANarbash::RightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bCanDoDamageRightWeapon && !bNarbashDied && !bNarbashStunned)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 1);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
			if(Character->GetBloodFXParticle())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Character->GetBloodFXParticle(),Character->GetActorLocation(),FRotator(0,0,0));
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 3);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}

		bCanDoDamageRightWeapon = false;
	}
}

void ANarbash::CombatRange()
{
	if(Character && !bNarbashDied && !bInvincibility)
	{
		bCombatRange = true;
		if(bCombatRange)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			const float NarbashAttackChance = FMath::FRandRange(0.f,1.f);
			GetCharacterMovement()->MaxWalkSpeed = 0;
			if(bCanAttack && NarbashCombatState == ENarbashCombatState::ENCS_Unoccupied && !bNarbashStunned)
			{
				if(NarbashAttackChance < 0.5f)
				{
					AnimInstance->Montage_Play(NarbashAttacks);
					AnimInstance->Montage_JumpToSection(FName("SecondAttack"));
					NarbashCombatState = ENarbashCombatState::ENCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else 
				{
					AnimInstance->Montage_Play(NarbashAttacks);
					AnimInstance->Montage_JumpToSection(FName("ThirdAttack"));
					NarbashCombatState = ENarbashCombatState::ENCS_FireTimerInProgress;
					bCanAttack = false;
				}
			}
			
		}
	}
}

/*void ANarbash::CombatRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bNarbashDied)
	{
		bCombatRange = false;
	}
}*/

void ANarbash::ThrowHammer()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if(Character)
		{
			FVector FXSpawnLocation(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z + 500.f);
			FRotator FXSpawnRotation(0,-90,0);
			GetWorld()->SpawnActor<AActor>(ThrowHammertoSpawn, FXSpawnLocation, FXSpawnRotation);
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("SpawnOldu"));
		}
	}
	else
	{
		FVector FXSpawnLocation(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z + 500.f);
		FRotator FXSpawnRotation(0,-90,0);
		GetWorld()->SpawnActor<AActor>(ThrowHammertoSpawn, FXSpawnLocation, FXSpawnRotation);
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("SpawnOldu"));
	}
}

void ANarbash::ThrowHammerTriggerFunc()
{
	FTimerHandle ThrowHammerTimer;
	GetWorldTimerManager().SetTimer(ThrowHammerTimer,this,&ANarbash::ThrowHammer,0.2f);
}

void ANarbash::ThrowHammerMontage()
{
	if(NarbashHammerState == ENarbashHammerState::ENHS_FirstHammer && NarbashHealth < 400)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(NarbashAttacks);
		AnimInstance->Montage_JumpToSection(FName("FourthAttack"));
		NarbashCombatState = ENarbashCombatState::ENCS_FireTimerInProgress;
		bCanAttack = false;
		NarbashHammerState = ENarbashHammerState::ENHS_SecondHammer;
		ThrowHammerTriggerFunc();
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if(NarbashHammerState == ENarbashHammerState::ENHS_SecondHammer && NarbashHealth < 300)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(NarbashAttacks);
		AnimInstance->Montage_JumpToSection(FName("FourthAttack"));
		NarbashCombatState = ENarbashCombatState::ENCS_FireTimerInProgress;
		bCanAttack = false;
		NarbashHammerState = ENarbashHammerState::ENHS_ThirdHammer;
		ThrowHammerTriggerFunc();
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	else if(NarbashHammerState == ENarbashHammerState::ENHS_ThirdHammer && NarbashHealth < 200)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(NarbashAttacks);
		AnimInstance->Montage_JumpToSection(FName("FourthAttack"));
		NarbashCombatState = ENarbashCombatState::ENCS_FireTimerInProgress;
		bCanAttack = false;
		NarbashHammerState = ENarbashHammerState::ENHS_EndHammer;
		ThrowHammerTriggerFunc();
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void ANarbash::SpawnFirstAbility()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		const FVector SpawnLocation(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z);
		const FRotator SpawnRotation(0,0,0);
		GetWorld()->SpawnActor<AActor>(MagicCapsuletoSpawn, SpawnLocation,SpawnRotation);
	}
	if(TriggerValue < 10)
	{
		TriggerValue++;
		FTimerHandle TriggerFirstSkill;
		GetWorldTimerManager().SetTimer(TriggerFirstSkill,this,&ANarbash::PlayFirstSkillAnim,2.f);
	}
	else
	{
		FTimerHandle TriggerEndInvincibility;
		GetWorldTimerManager().SetTimer(TriggerEndInvincibility,this,&ANarbash::EndInvincibility,2.f);
	}
}

void ANarbash::PlayFirstSkillAnim()
{
	NarbashCombatState = ENarbashCombatState::ENCS_Unoccupied;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(NarbashCombatState == ENarbashCombatState::ENCS_Unoccupied && !bNarbashDied)
	{
		bInvincibility = true;
		FirstSkillUsed = true;
		AnimInstance->Montage_Play(NarbashAttacks);
		AnimInstance->Montage_JumpToSection(FName("FirstAttack"));
		NarbashCombatState = ENarbashCombatState::ENCS_FireTimerInProgress;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		SpawnFirstAbility();
		NarbashHealth = NarbashHealth + 20.f;
		AuraParticleSystemComponent->SetVisibility(true);
	}
}

void ANarbash::EndInvincibility()
{
	bCanAttack = true;
	NarbashCombatState = ENarbashCombatState::ENCS_Unoccupied;
	bInvincibility = false;
	GetCharacterMovement()->MaxWalkSpeed = 800;
	AuraParticleSystemComponent->SetVisibility(false);
}

void ANarbash::SpawnSafeZone()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		const float RandomLocation = FMath::RandRange(-600.f, 600.f);
		const FVector SpawnLocationSafeZone(GetActorLocation().X + RandomLocation,GetActorLocation().Y+RandomLocation,GetActorLocation().Z);
		float Distance = FVector::Distance(GetActorLocation(), SpawnLocationSafeZone);
		if(Distance < 200)
		{
			return SpawnSafeZone();
		}
		const FRotator SpawnRotation(0,0,0);
		GetWorld()->SpawnActor<AActor>(SafeZonetoSpawn, SpawnLocationSafeZone,SpawnRotation);
		if(TriggerSafeZoneValue < 10)
		{
			TriggerSafeZoneValue++;
			GetWorldTimerManager().SetTimer(TriggerSafeZone,this,&ANarbash::SpawnSafeZone,2.f);
		}
	}
}

void ANarbash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		CombatRangeDistance = FVector::Distance(GetActorLocation(),Character->GetActorLocation());
		if(CombatRangeDistance < 175.f)
		{
			bCombatRange = true;
			CombatRange();
		}
		else
		{
			bCombatRange = false;
		}
	}
	if(Character && !bNarbashDied)
	{
		if(WasRecentlyRendered(0.2f))
		{
			Character->SetNarbashRendered(true);
		}
		else
		{
			Character->SetNarbashRendered(false);
		}
	}
	if(!bNarbashDied && NarbashHealth <= 0)
	{
		bNarbashDied = true;
		NarbashHealth = 0;
		Die();
	}
	if(NarbashHealth <= 200 && !FirstSkillUsed && NarbashCombatState == ENarbashCombatState::ENCS_Unoccupied)
	{
		FirstSkillUsed = true;
		PlayFirstSkillAnim();
		GetWorldTimerManager().SetTimer(TriggerSafeZone,this,&ANarbash::SpawnSafeZone,2.f);
	}
}


