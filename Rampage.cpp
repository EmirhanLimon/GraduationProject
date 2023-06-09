
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
	bRightArmCanDamage(false),
	RampageHealth(750.f),
	bTraining(true),
	bCanJump(false),
	JumpAmount(0),
	bMovementStop(false),
	bFirstJump(false),
	bSecondJump(false),
	bFirstJumping(false),
	bSecondJumping(false),
	bBehind(false)
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
	PlayTrainingMontage();
	/*FTimerHandle aaa;
	GetWorldTimerManager().SetTimer(aaa,this,&ARampage::PlayThrowStoneAnimMontage,5.f);*/
	GetWorldTimerManager().SetTimer(bCanJumpTrigger,this,&ARampage::CanJump,20.f);
}

void ARampage::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && !bRampageDied && RampageCombatState == ERampageCombatState::ERCS_Unoccupied && !bIsInAir && !bTraining)
	{
		if(!IsValid(AIC_Ref))
		{
			AIC_Ref = Cast<AEnemyController>(GetController());
		}
		else
		{
			if(bMovementStop && bFirstJumping)
			{
				AIC_Ref->MoveToLocation(FirstJumpTargetLocation, -1.f);
				GetCharacterMovement()->MaxWalkSpeed = 1000;
				GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT("12312"));
			}
			if(bMovementStop && bSecondJumping)
			{
				AIC_Ref->MoveToLocation(SecondJumpTargetLocation, -1.f);
				GetCharacterMovement()->MaxWalkSpeed = 1000;
				GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT("12312"));
			}
			if(!bMovementStop)
			{
				AIC_Ref->MoveToLocation(Character->GetActorLocation(), -1.f);
				GetCharacterMovement()->MaxWalkSpeed = 1000;
				GEngine->AddOnScreenDebugMessage(-1,0.1,FColor::Blue,TEXT("12312"));
			}
		}
	}
}

void ARampage::CanJump()
{
	if(JumpAmount <= 1)
	{
		GetWorldTimerManager().SetTimer(bCanJumpTrigger,this,&ARampage::CanJump,15.f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(bCanJumpTrigger);
	}
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::White,TEXT("Tetiklendi."));
	bCanJump = true;
}

void ARampage::PlayTrainingMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(TrainingMontage);
		AnimInstance->Montage_JumpToSection(FName("Training"));
	}
	UGameplayStatics::PlaySound2D(this,TrainingSoundCue);
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
		UGameplayStatics::PlaySound2D(this,ThrowStoneSoundCue);
		AnimInstance->Montage_Play(ThrowStoneMontage);
		AnimInstance->Montage_JumpToSection(FName("ThrowStone"));
		FTimerHandle SpawnStoneTimer;
		GetWorldTimerManager().SetTimer(SpawnStoneTimer, this, &ARampage::ThrowStone, 0.4f);
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
			UGameplayStatics::PlaySound2D(this,Character->GetStoneHitWorldSoundCue());
			bJumping = false;
			bMovementStop = false;
			bCanJump = false;
			GetWorldTimerManager().ClearTimer(bCanJumpTrigger);
			GetWorldTimerManager().SetTimer(bCanJumpTrigger,this,&ARampage::CanJump,15.f);
			if(bFirstJumping)
			{
				bFirstJumping = false;
			}
			if(bSecondJumping)
			{
				bSecondJumping = false;
			}
			PlayThrowStoneAnimMontage();
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
			UGameplayStatics::PlaySound2D(this,ThrowStoneSoundCue);
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("GroundSmash"));
			FTimerHandle SpawnGroundSmashTimer;
			GetWorldTimerManager().SetTimer(SpawnGroundSmashTimer, this ,&ARampage::GroundSmashSpawn,1.f);	
		}
		else if(RandomAttackState >= 0.2f && RandomAttackState < 0.4f)
		{
			UGameplayStatics::PlaySound2D(this,AttackSoundCue);
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("FirstBasicAttack"));
		}
		else if(RandomAttackState >= 0.4f && RandomAttackState < 0.6f)
		{
			UGameplayStatics::PlaySound2D(this,AttackSoundCue);
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("SecondBasicAttack"));
		}
		else if(RandomAttackState >= 0.6f && RandomAttackState < 0.8f)
		{
			UGameplayStatics::PlaySound2D(this,AttackSoundCue);
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("ThirdBasicAttack"));
		}
		else
		{
			UGameplayStatics::PlaySound2D(this,AttackSoundCue);
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
	/*if(!IsValid(AIC_Ref))
	{
		AIC_Ref = Cast<AEnemyController>(GetController());
	}
	else
	{
		if(bFirstJumping)
		{
			AIC_Ref->MoveToLocation(FirstJumpTargetLocation, -1.f);
			GetCharacterMovement()->MaxWalkSpeed = 1000;
		}
		if(bSecondJumping)
		{
			AIC_Ref->MoveToLocation(FirstJumpTargetLocation, -1.f);
			GetCharacterMovement()->MaxWalkSpeed = 1000;
		}
	}*/
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("birinci zıplama oldu"));
	JumpAmount++;
	Super::Jump();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		AnimInstance->Montage_Play(JumpMontage);
		AnimInstance->Montage_JumpToSection(FName("Jump"));
		UGameplayStatics::PlaySound2D(this,JumpSoundCue);
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
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 5.f);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 5.f);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
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
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 5.f);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 5.f);
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
		FirstJumpDistance = FVector::Distance(GetActorLocation(),FirstJumpTargetLocation);
		SecondJumpDistance = FVector::Distance(GetActorLocation(),SecondJumpTargetLocation);
		Distance = FVector::Distance(GetActorLocation(), Character->GetActorLocation());
		if(bIsInAir)
		{
			if((FirstJumpDistance > 1000.f && bFirstJumping) || (SecondJumpDistance > 1000.f && bSecondJumping))
			{
				if(bFirstJump && !bSecondJump)
				{
					ClampValue =  FMath::Clamp((Alpha + 0.2f),0,1);
					Alpha = ClampValue;
					FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),FirstJumpTargetLocation);
					FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
					SetActorRotation(FRotator(GetActorRotation().Pitch,NewRotation.Yaw,NewRotation.Roll));	
				}
				else
				{
					ClampValue =  FMath::Clamp((Alpha + 0.2f),0,1);
					Alpha = ClampValue;
					FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),SecondJumpTargetLocation);
					FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
					SetActorRotation(FRotator(GetActorRotation().Pitch,NewRotation.Yaw,NewRotation.Roll));
				}
				SetActorLocation(GetActorLocation() + (GetActorForwardVector() * (Distance * 0.8f) * DeltaTime));
			}
		}
		if(bJumping && !bIsInAir)
		{
			PlayJumpEndFX();
		}
		if(!bIsInAir && !bJumping && RampageCombatState == ERampageCombatState::ERCS_Unoccupied && Distance < 600.f && bCanAttack && !bMovementStop)
		{
			GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Blue,TEXT("131233213"));
			Attack();
			RampageCombatState = ERampageCombatState::ERCS_FireTimerInProgress;
			bCanAttack = false;
		}
		if(!bIsInAir && !bJumping && RampageCombatState == ERampageCombatState::ERCS_Unoccupied && bCanJump)
		{
			if(JumpAmount == 0)
			{
				//float FirstJumpDistance = FVector::Distance(GetActorLocation(),FirstJumpTargetLocation);
				if(FirstJumpDistance > 5500 && !bFirstJump)
				{
					bCanJump = false;
					bFirstJumping = true;
					bFirstJump = true;
					bMovementStop = true;
					FTimerHandle ccc;
					GetWorldTimerManager().SetTimer(ccc,this,&ARampage::Jump,1.f);
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("birinci zıplama"));
				}
			}
			else
			{
				//float SecondJumpDistance = FVector::Distance(GetActorLocation(),SecondJumpTargetLocation);
				if(SecondJumpDistance > 5500 && !bSecondJump)
				{
					bCanJump = false;
					bSecondJumping = true;
					bSecondJump = true;
					bMovementStop = true;
					FTimerHandle ccc;
					GetWorldTimerManager().SetTimer(ccc,this,&ARampage::Jump,1.f);
					GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("ikinci zıplama"));
				}
			}

		}
	}
}



