
#include "Fey.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AFey::AFey() :
	bCanAttack(true),
	bCombatRange(false),
	FeyHealth(60.f),
	bFeyDied(false),
	FeyCombatState(EFeyCombatState::EFCS_Unoccupied),
	Distance(0.f)
{

	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	
}

void AFey::BeginPlay()
{
	Super::BeginPlay();
	AIC_Ref = Cast<AEnemyController>(GetController());
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PawnSensing->OnHearNoise.AddDynamic(this, &AFey::OnHearNoise);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AFey::OnHearNoise(APawn* OtherActor, const FVector& Location, float Volume)
{
	if(OtherActor == nullptr) return;
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character && FeyCombatState == EFeyCombatState::EFCS_Unoccupied && !bCombatRange && !bFeyDied)
	{
		FVector CharacterLocation(Character->GetActorLocation().X, Character->GetActorLocation().Y ,Character->GetActorLocation().Z + 200.f);
		AIC_Ref->MoveToLocation(CharacterLocation, -1.f,true,false);
		GetCharacterMovement()->MaxFlySpeed = 900;
	}
	else
	{
		AIC_Ref->StopMovement();
	}
}

void AFey::CombatRange()
{
	Distance = FVector::Distance(Character->GetActorLocation(), GetActorLocation());
	if(Character && Distance < 1500)
	{
		bCombatRange = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(bCombatRange && AnimInstance && FeyCombatState == EFeyCombatState::EFCS_Unoccupied && bCanAttack)
		{

			FTimerHandle SpawnMagicBallTimer;
			GetCharacterMovement()->MaxFlySpeed = 0;
			const float RandomAttack = FMath::RandRange(0.f, 1.f);
			if(RandomAttack < 0.2f)
			{
				AnimInstance->Montage_Play(FeyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("FirstAttack"));
				GetWorldTimerManager().SetTimer(SpawnMagicBallTimer, this, &AFey::SpawnMagicBallLeft, 0.13f);
			}
			else if(RandomAttack > 0.2f && RandomAttack < 0.5f)
			{
				AnimInstance->Montage_Play(FeyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("SecondAttack"));
				GetWorldTimerManager().SetTimer(SpawnMagicBallTimer, this, &AFey::Attack, 0.15f);
			}
			else if(RandomAttack > 0.5f && RandomAttack < 0.75f)
			{
				AnimInstance->Montage_Play(FeyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("ThirdAttack"));
				GetWorldTimerManager().SetTimer(SpawnMagicBallTimer, this, &AFey::SpawnMagicBallRight, 0.18f);
			}
			else if(RandomAttack > 0.75f && RandomAttack < 1.f)
			{
				AnimInstance->Montage_Play(FeyAttackMontage);
				AnimInstance->Montage_JumpToSection(FName("FourthAttack"));
				GetWorldTimerManager().SetTimer(SpawnMagicBallTimer, this, &AFey::SpawnMagicBallLeft, 0.10f);
			}
			FeyCombatState = EFeyCombatState::EFCS_FireTimerInProgress;
			bCanAttack = false;
		}
	}
	else
	{
		bCombatRange = false;
		FeyCombatState = EFeyCombatState::EFCS_Unoccupied;
	}
}

void AFey::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		AnimInstance->Montage_Play(FeyDieMontage);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	FTimerHandle DieTimer;
	GetWorldTimerManager().SetTimer(DieTimer, this, &AFey::Destroyed, 1.8f);
}

void AFey::SpawnMagicBallLeft()
{
	const USkeletalMeshSocket* FeySpawnSocket = GetMesh()->GetSocketByName(FName("FX_Trail_L_02"));
	if(FeySpawnSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("1231321124141421"));
		const FTransform SocketTransform = FeySpawnSocket->GetSocketTransform(GetMesh());
		FVector FXSpawnLocation = SocketTransform.GetLocation();
		FRotator FXSpawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AActor>(MagicBallToSpawn, FXSpawnLocation, FXSpawnRotation);
	}

}

void AFey::SpawnMagicBallRight()
{
	const USkeletalMeshSocket* FeySpawnSocket = GetMesh()->GetSocketByName(FName("FX_Trail_R_02"));
	if(FeySpawnSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("1231321124141421"));
		const FTransform SocketTransform = FeySpawnSocket->GetSocketTransform(GetMesh());
		FVector FXSpawnLocation = SocketTransform.GetLocation();
		FRotator FXSpawnRotation = GetActorRotation();
		GetWorld()->SpawnActor<AActor>(MagicBallToSpawn, FXSpawnLocation, FXSpawnRotation);
	}

}

void AFey::Attack()
{
	const USkeletalMeshSocket* FeySpawnSocket = GetMesh()->GetSocketByName(FName("FX_Trail_R_02"));
	if(AttackParticle && FeySpawnSocket)
	{
		const FTransform SocketTransform = FeySpawnSocket->GetSocketTransform(GetMesh());
		FVector FXSpawnLocation = SocketTransform.GetLocation();
		FRotator FXSpawnRotation = GetActorRotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle,FXSpawnLocation ,FXSpawnRotation);
	}
	
}

void AFey::Destroyed()
{
	Destroy();
}

void AFey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CombatRange();
	if(Character && this->WasRecentlyRendered(0.2f))
	{
		Character->SetFeyRendered(true);
	}
	else
	{
		Character->SetFeyRendered(false);
	}

	
	if(bCombatRange && Character)
	{
		ClampValue =  FMath::Clamp((Alpha + 0.01f),0,1);
		Alpha = ClampValue;
		FVector TargetLocation(Character->GetActorLocation().X,Character->GetActorLocation().Y,GetActorLocation().Z);
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
		FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
		SetActorRotation(NewRotation);
	}
	else
	{
		Alpha = 0;
		ClampValue = 0;
	}
	if(!bFeyDied && FeyHealth <= 0)
	{
		bFeyDied = true;
		FeyHealth = 0;
		Die();
	}
}


