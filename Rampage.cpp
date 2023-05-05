
#include "Rampage.h"

#include "EnemyController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"


ARampage::ARampage() :
	bIsInAir(false),
	bRampageDied(false),
	RampageCombatState(ERampageCombatState::ERCS_Unoccupied),
	bJumping(false),
	bCanAttack(true),
	bLeftArmCanDamage(false),
	bRightArmCanDamage(false)
{

	PrimaryActorTick.bCanEverTick = true;

	RampagePawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("RampagePawnSensing"));

	LeftArmBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftArmBoxComponent"));
	LeftArmBoxComponent->SetupAttachment(GetMesh(), FName("FX_Trail_L_03"));
	RightArmBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RightArmBoxComponent"));
	RightArmBoxComponent->SetupAttachment(GetMesh(), FName("FX_Trail_R_03"));
}


void ARampage::BeginPlay()
{
	Super::BeginPlay();
	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	RampagePawnSensing->OnHearNoise.AddDynamic(this,&ARampage::OnHearNoise);

	LeftArmBoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ARampage::LeftArmOverlap);
	RightArmBoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ARampage::RightArmOverlap);
	
	FTimerHandle aaa;
	GetWorldTimerManager().SetTimer(aaa,this,&ARampage::PlayThrowStoneAnimMontage,3.f);
	FTimerHandle bbb;
	GetWorldTimerManager().SetTimer(bbb,this,&ARampage::Jump,6.f);
}

void ARampage::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bRampageDied && RampageCombatState == ERampageCombatState::ERCS_Unoccupied && !bIsInAir)
	{
		AIC_Ref->MoveToLocation(Character->GetActorLocation(), -1.f);
		GetCharacterMovement()->MaxWalkSpeed = 1000;
	}
}

void ARampage::ThrowStone()
{
	const USkeletalMeshSocket* StoneSpawnSocket = GetMesh()->GetSocketByName(FName("FX_Trail_L_02"));
	const FTransform SocketTransform = StoneSpawnSocket->GetSocketTransform(GetMesh());
	const FVector StoneSpawnLocation = SocketTransform.GetLocation();
	const FRotator StoneSpawnRotation(90,0,0);
	if(StonetoSpawn)
	{
		GetWorld()->SpawnActor<AActor>(StonetoSpawn,StoneSpawnLocation, StoneSpawnRotation);
	}
}

void ARampage::PlayThrowStoneAnimMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && RampageCombatState == ERampageCombatState::ERCS_Unoccupied && !bRampageDied)
	{
		AnimInstance->Montage_Play(ThrowStoneMontage);
		AnimInstance->Montage_JumpToSection(FName("ThrowStone"));
		FTimerHandle SpawnStoneTimer;
		GetWorldTimerManager().SetTimer(SpawnStoneTimer, this, &ARampage::ThrowStone, 0.3f);
	}
}

void ARampage::Jumping()
{
	bJumping = true;
}

void ARampage::PlayJumpEndFX()
{
	const USkeletalMeshSocket* JumpEndFXSpawnSocket = GetMesh()->GetSocketByName(FName("FX_FootImpact_L"));
	const FTransform SocketTransform = JumpEndFXSpawnSocket->GetSocketTransform(GetMesh());
	const FVector JumpEndFXSpawnLocation = SocketTransform.GetLocation();
	const FRotator JumpEndFXSpawnRotation(0,0,0);
	if(JumpEndFxParticle)
	{

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpEndFxParticle,JumpEndFXSpawnLocation ,JumpEndFXSpawnRotation);
		if(CameraShakeJumpEnd && Character->GetCameraManager() && Character)
		{
			Character->GetCameraManager()->StartCameraShake(CameraShakeJumpEnd,1);
			bJumping = false;
		}
	}
}

void ARampage::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		const float RandomAttackState = FMath::FRandRange(0.f,1.f);
		if(RandomAttackState >= 0.f && RandomAttackState < 0.2f)
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("GroundSmash"));
			FTimerHandle SpawnGroundSmashTimer;
			GetWorldTimerManager().SetTimer(SpawnGroundSmashTimer, this ,&ARampage::GroundSmashSpawn,1.f);	
		}
		else if(RandomAttackState >= 0.2f && RandomAttackState < 0.4f)
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("FirstBasicAttack"));
		}
		else if(RandomAttackState >= 0.4f && RandomAttackState < 0.6f)
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("SecondBasicAttack"));
		}
		else if(RandomAttackState >= 0.6f && RandomAttackState < 0.8f)
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("ThirdBasicAttack"));
		}
		else
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("FourthBasicAttack"));
		}
	}
}

void ARampage::GroundSmashSpawn()
{
	const USkeletalMeshSocket* GroundSmashSpawnSocket = GetMesh()->GetSocketByName(FName("FX_Trail_R_03"));
	if(GroundSmashSpawnSocket && GetMesh())
	{
		const FTransform SocketTransform = GroundSmashSpawnSocket->GetSocketTransform(GetMesh());
		const FVector GroundSmashSpawnSocketLocation = SocketTransform.GetLocation();
		const FRotator GroundSmashSpawnSocketRotation(0,0,0);
		if(GroundSmashtoSpawn && GetWorld())
		{
			GetWorld()->SpawnActor<AActor>(GroundSmashtoSpawn,GroundSmashSpawnSocketLocation,GroundSmashSpawnSocketRotation);
		}
	}
}

void ARampage::Jump()
{
	Super::Jump();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(JumpMontage);
		AnimInstance->Montage_JumpToSection(FName("Jump"));
	}
	FTimerHandle JumpingTimer;
	GetWorldTimerManager().SetTimer(JumpingTimer,this,&ARampage::Jumping,0.5f);
}

void ARampage::LeftArmOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bLeftArmCanDamage)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 7.5f);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 7.5f);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
		bLeftArmCanDamage = false;
	}
	
}

void ARampage::RightArmOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && bRightArmCanDamage)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 7.5f);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 7.5f);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
		bRightArmCanDamage = false;
	}	
}

void ARampage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character == nullptr)
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	if(Character)
	{
		if(WasRecentlyRendered(0.2f))
		{
			Character->SetRampageRendered(true);
		}
		else
		{
			Character->SetRampageRendered(false);
		}
		Distance = FVector::Distance(GetActorLocation(),Character->GetActorLocation());
		if(bIsInAir)
		{
			if(Distance > 1000.f)
			{
				ClampValue =  FMath::Clamp((Alpha + 0.2f),0,1);
				Alpha = ClampValue;
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Character->GetActorLocation());
				FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
				SetActorRotation(FRotator(GetActorRotation().Pitch,NewRotation.Yaw,NewRotation.Roll));	
			}
			SetActorLocation(GetActorLocation() + (GetActorForwardVector() * (Distance) * DeltaTime));
		}
		if(bJumping && !bIsInAir)
		{
			PlayJumpEndFX();
		}
		if(!bIsInAir && !bJumping && RampageCombatState == ERampageCombatState::ERCS_Unoccupied && Distance < 500.f && bCanAttack)
		{
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue,TEXT("131233213"));
			Attack();
			RampageCombatState = ERampageCombatState::ERCS_FireTimerInProgress;
			bCanAttack = false;
		}
	}
}

