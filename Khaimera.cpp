


#include "Khaimera.h"

#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AKhaimera::AKhaimera() :
	bKhaimeraStunned(false),
	KhaimeraHealth(90.f),
	bCombatRange(false),
	KhaimeraCombatState(EKhaimeraCombatState::EKCS_Unoccupied),
	bCanAttack(true),
	bCanDoDamageLeftWeapon(false),
	bCanDoDamageRightWeapon(false),
	bKhaimeraDied(false),
	bBehind(false)
{
 	
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	CombatRangeSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRangeSphereComponent"));
	CombatRangeSphereComponent->SetupAttachment(GetRootComponent());

	LeftWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftWeaponCollision"));
	LeftWeaponCollision->SetupAttachment(GetMesh(), FName("weapon_l"));

	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightWeaponCollision"));
	RightWeaponCollision->SetupAttachment(GetMesh(), FName("weapon_r"));
}


void AKhaimera::BeginPlay()
{
	Super::BeginPlay();

	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PawnSensing->OnHearNoise.AddDynamic(this, &AKhaimera::OnHearNoise);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AKhaimera::Patrol, 2.f);

	LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AKhaimera::LeftWeaponOverlap);
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AKhaimera::RightWeaponOverlap);
	CombatRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AKhaimera::CombatRangeOverlap);
	CombatRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AKhaimera::CombatRangeOverlapEnd);
}

void AKhaimera::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && KhaimeraCombatState == EKhaimeraCombatState::EKCS_Unoccupied && !bKhaimeraDied && !bKhaimeraStunned)
	{
		if(!IsValid(AIC_Ref))
		{
			AIC_Ref = Cast<AEnemyController>(GetController());
		}
		else
		{
			AIC_Ref->MoveToLocation(Character->GetActorLocation(), -1.f);
			GetWorldTimerManager().ClearTimer(PatrolTimer);
			GetCharacterMovement()->MaxWalkSpeed = 800;
		}
	}
}

void AKhaimera::Patrol()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if(NavSys)
	{
		FNavLocation NavLoc;
		NavSys->GetRandomReachablePointInRadius(GetActorLocation(), 1500.f, NavLoc);
		if(!IsValid(AIC_Ref))
		{
			AIC_Ref = Cast<AEnemyController>(GetController());
			if (AIC_Ref && !bCombatRange && !bKhaimeraDied && !bKhaimeraStunned)
			{
				AIC_Ref->MoveToLocation(NavLoc);
				GetWorldTimerManager().SetTimer(PatrolTimer, this, &AKhaimera::Patrol, 5.f);
				GetCharacterMovement()->MaxWalkSpeed = 600;
			}
		}
		else
		{
			if (AIC_Ref && !bCombatRange && !bKhaimeraDied && !bKhaimeraStunned)
			{
				AIC_Ref->MoveToLocation(NavLoc);
				GetWorldTimerManager().SetTimer(PatrolTimer, this, &AKhaimera::Patrol, 5.f);
				GetCharacterMovement()->MaxWalkSpeed = 600;
			}
		}
		
	}
}

void AKhaimera::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		FTimerHandle DieTimer;
		const float RandomDie = FMath::RandRange(0.f,1.f);
		if(RandomDie <= 0.5f)
		{
			AnimInstance->Montage_Play(KhaimeraDieMontage);
			AnimInstance->Montage_JumpToSection(FName("DeathA"));
			GetWorldTimerManager().SetTimer(DieTimer, this, &AKhaimera::Destroyed, 1.1f);
		}
		else
		{
			AnimInstance->Montage_Play(KhaimeraDieMontage);
			AnimInstance->Montage_JumpToSection(FName("DeathB"));
			GetWorldTimerManager().SetTimer(DieTimer, this, &AKhaimera::Destroyed, 0.7f);
		}
	}
}

void AKhaimera::Destroyed()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		/*if(Character)
		{
			Character->SetAmountOfDeadEnemies(Character->GetAmountOfDeadEnemies() + 1);
			const float RandomPotion = FMath::FRandRange(0.f,1.f);
			if(RandomPotion < 0.5f)
			{
				GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT("Mana İksiri"));
				GetWorld()->SpawnActor<AActor>(Character->GetManaPotion(), (GetActorLocation() + GetActorForwardVector() * -100.f), GetActorRotation());
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("Can İksiri"));
				GetWorld()->SpawnActor<AActor>(Character->GetHealthPotion(), (GetActorLocation() + GetActorForwardVector() * -100.f), GetActorRotation());
			}
		}*/
	}
	else
	{
		Character->SetAmountOfDeadEnemies(Character->GetAmountOfDeadEnemies() + 1);
		const float RandomPotion = FMath::FRandRange(0.f,1.f);
		if(RandomPotion < 0.5f)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT("Mana İksiri"));
			GetWorld()->SpawnActor<AActor>(Character->GetManaPotion(), (GetActorLocation() + GetActorForwardVector() * -100.f), GetActorRotation());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("Can İksiri"));
			GetWorld()->SpawnActor<AActor>(Character->GetHealthPotion(), (GetActorLocation() + GetActorForwardVector() * -100.f), GetActorRotation());
		}
		
	}
	Destroy();
}

void AKhaimera::LeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bCanDoDamageLeftWeapon && !bKhaimeraDied && !bKhaimeraStunned)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 5);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 5);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
		if(Character->GetBloodFXParticle())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Character->GetBloodFXParticle(),Character->GetActorLocation(),FRotator(0,0,0));
		}
		
		bCanDoDamageLeftWeapon = false;
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(Character->GetCombatState() == ECombatState::ECS_Unoccupied && !Character->GetIsInAir() && !Character->GetRolling() && !Character->GetCharacterChanging())
		{
			if(Character->GetCameraManager())
			{
				Character->GetCameraManager()->StartCameraShake(Character->GetCameraShakeHitReact(),1);
			}
			const float HitReacts = FMath::RandRange(0.f, 1.f);
			if(Character->GetCharacterState() == ECharacterState::ECS_Warrior && HitReacts <= 0.75f)
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
			if(Character->GetCharacterState() == ECharacterState::ECS_Archer && HitReacts <= 0.75f)
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

void AKhaimera::RightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bCanDoDamageRightWeapon && !bKhaimeraDied && !bKhaimeraStunned)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 5);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 5);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
		if(Character->GetBloodFXParticle())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Character->GetBloodFXParticle(),Character->GetActorLocation(),FRotator(0,0,0));
		}
		bCanDoDamageRightWeapon = false;
	}
}

void AKhaimera::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bKhaimeraDied)
	{
		bCombatRange = true;
		if(bCombatRange)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			const float KhaimeraAttackChance = FMath::FRandRange(0.f,1.f);
			GetCharacterMovement()->MaxWalkSpeed = 0;
			if(bCanAttack && KhaimeraCombatState == EKhaimeraCombatState::EKCS_Unoccupied && !bKhaimeraStunned)
			{
				if(KhaimeraAttackChance >= 0.f && KhaimeraAttackChance < 0.35f)
				{
					AnimInstance->Montage_Play(KhaimeraAttacks);
					AnimInstance->Montage_JumpToSection(FName("LeftStrike"));
					KhaimeraCombatState = EKhaimeraCombatState::EKCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else if(KhaimeraAttackChance >= 0.35f && KhaimeraAttackChance < 0.70f)
				{
					AnimInstance->Montage_Play(KhaimeraAttacks);
					AnimInstance->Montage_JumpToSection(FName("RightStrike"));
					KhaimeraCombatState = EKhaimeraCombatState::EKCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else if(KhaimeraAttackChance >= 0.70f && KhaimeraAttackChance < 0.90f)
				{
					AnimInstance->Montage_Play(KhaimeraAttacks);
					AnimInstance->Montage_JumpToSection(FName("DoublePainStrike"));
					KhaimeraCombatState = EKhaimeraCombatState::EKCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else if(KhaimeraAttackChance >= 0.90f && KhaimeraAttackChance <= 1.f)
				{
					AnimInstance->Montage_Play(KhaimeraAttacks);
					AnimInstance->Montage_JumpToSection(FName("QuadStrike"));
					KhaimeraCombatState = EKhaimeraCombatState::EKCS_FireTimerInProgress;
					bCanAttack = false;
				}
			}
			
		}
	}
}

void AKhaimera::CombatRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bKhaimeraDied)
	{
		bCombatRange = false;
	}
}


void AKhaimera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character && !bKhaimeraDied)
	{
		if(WasRecentlyRendered(0.2f))
		{
			Character->SetKhaimeraRendered(true);
		}
		else
		{
			Character->SetKhaimeraRendered(false);
		}
	}
	if(!bKhaimeraDied && KhaimeraHealth <= 0)
	{
		bKhaimeraDied = true;
		KhaimeraHealth = 0;
		Die();
	}
}


