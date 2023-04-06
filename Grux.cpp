


#include "Grux.h"

#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AGrux::AGrux() :
	bGruxStunned(false),
	GruxHealth(130.f),
	bCombatRange(false),
	GruxCombatState(EGruxCombatState::EGCS_Unoccupied),
	bCanAttack(true),
	bCanDoDamageLeftWeapon(false),
	bCanDoDamageRightWeapon(false),
	bGruxDied(false),
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

void AGrux::BeginPlay()
{
	Super::BeginPlay();
	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PawnSensing->OnHearNoise.AddDynamic(this, &AGrux::OnHearNoise);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AGrux::Patrol, 2.f);

	LeftWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrux::LeftWeaponOverlap);
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrux::RightWeaponOverlap);
	CombatRangeSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AGrux::CombatRangeOverlap);
	CombatRangeSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AGrux::CombatRangeOverlapEnd);
}

void AGrux::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && GruxCombatState == EGruxCombatState::EGCS_Unoccupied && !bGruxDied && !bGruxStunned)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("1231"));
		AIC_Ref->MoveToLocation(Character->GetActorLocation(), -1.f);
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 600;
	}
}

void AGrux::Patrol()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if(NavSys)
	{
		FNavLocation NavLoc;
		NavSys->GetRandomReachablePointInRadius(GetActorLocation(), 1500.f, NavLoc);
		if (AIC_Ref && !bCombatRange && !bGruxDied && !bGruxStunned)
		{
			AIC_Ref->MoveToLocation(NavLoc);
			GetWorldTimerManager().SetTimer(PatrolTimer, this, &AGrux::Patrol, 7.5f);
			GetCharacterMovement()->MaxWalkSpeed = 400;
		}
	}
}

void AGrux::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		const float RandomDie = FMath::RandRange(0.f,1.f);
		if(RandomDie <= 0.5f)
		{
			AnimInstance->Montage_Play(GruxDieMontage);
			AnimInstance->Montage_JumpToSection(FName("DeathA"));
		}
		else
		{
			AnimInstance->Montage_Play(GruxDieMontage);
			AnimInstance->Montage_JumpToSection(FName("DeathB"));
		}
	}
	FTimerHandle DieTimer;
	GetWorldTimerManager().SetTimer(DieTimer, this, &AGrux::Destroyed, 1.4f);
}

void AGrux::Destroyed()
{
	Destroy();
}

void AGrux::LeftWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bCanDoDamageLeftWeapon && !bGruxDied && !bGruxStunned)
	{
		Character->SetCharacterHealth(Character->GetCharacterHealth() - 3);
		bCanDoDamageLeftWeapon = false;
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if(Character->GetCombatState() == ECombatState::ECS_Unoccupied && !Character->GetIsInAir() && !Character->GetRolling() && !Character->GetCharacterChanging())
		{
			const float HitReacts = FMath::RandRange(0.f, 1.f);
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT("12321"));
			if(Character->GetCharacterState() == ECharacterState::ECS_Warrior && HitReacts <= 0.5f)
			{
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
			if(Character->GetCharacterState() == ECharacterState::ECS_Archer && HitReacts <= 0.5f)
			{
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

void AGrux::RightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bCanDoDamageRightWeapon && !bGruxDied && !bGruxStunned)
	{
		Character->SetCharacterHealth(Character->GetCharacterHealth() - 3);
		bCanDoDamageRightWeapon = false;
	}
}

void AGrux::CombatRangeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bGruxDied)
	{
		bCombatRange = true;
		if(bCombatRange)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("sdada"));
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			const float GruxAttackChance = FMath::FRandRange(0.f,1.f);
			GetCharacterMovement()->MaxWalkSpeed = 0;
			if(bCanAttack && GruxCombatState == EGruxCombatState::EGCS_Unoccupied && !bGruxStunned)
			{
				if(GruxAttackChance >= 0.f && GruxAttackChance < 0.35f)
				{
					AnimInstance->Montage_Play(GruxAttacks);
					AnimInstance->Montage_JumpToSection(FName("LeftStrike"));
					GruxCombatState = EGruxCombatState::EGCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else if(GruxAttackChance >= 0.35f && GruxAttackChance < 0.70f)
				{
					AnimInstance->Montage_Play(GruxAttacks);
					AnimInstance->Montage_JumpToSection(FName("RightStrike"));
					GruxCombatState = EGruxCombatState::EGCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else if(GruxAttackChance >= 0.70f && GruxAttackChance < 0.90f)
				{
					AnimInstance->Montage_Play(GruxAttacks);
					AnimInstance->Montage_JumpToSection(FName("DoublePainStrike"));
					GruxCombatState = EGruxCombatState::EGCS_FireTimerInProgress;
					bCanAttack = false;
				}
				else if(GruxAttackChance >= 0.90f && GruxAttackChance <= 1.f)
				{
					AnimInstance->Montage_Play(GruxAttacks);
					AnimInstance->Montage_JumpToSection(FName("QuadStrike"));
					GruxCombatState = EGruxCombatState::EGCS_FireTimerInProgress;
					bCanAttack = false;
				}
			}
			
		}
	}
}

void AGrux::CombatRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bGruxDied)
	{
		bCombatRange = false;
	}
}

void AGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character && !bGruxDied)
	{
		if(WasRecentlyRendered(0.2f))
		{
			Character->SetGruxRendered(true);
		}
		else
		{
			Character->SetGruxRendered(false);
		}
	}
	if(!bGruxDied && GruxHealth <= 0)
	{
		bGruxDied = true;
		GruxHealth = 0;
		Die();
	}
}

