


#include "MagicBall.h"

#include "NavigationSystemTypes.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


AMagicBall::AMagicBall() :
	Alpha(0.f),
	ClampValue(0.f)
{

	PrimaryActorTick.bCanEverTick = true;

	MagicBallSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MagicBallSphere"));
	MagicBallSphere->SetupAttachment(GetRootComponent());

	MagicBallParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicBallParticle"));
	MagicBallParticleSystemComponent->SetupAttachment(MagicBallSphere);
}


void AMagicBall::BeginPlay()
{
	Super::BeginPlay();
	Attack();
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	TargetLocation = Character->GetActorLocation();
	MagicBallSphere->OnComponentBeginOverlap.AddDynamic(this, &AMagicBall::AttackOverlap);
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AMagicBall::Destroyed, 1.f);
}

void AMagicBall::Attack()
{

}

void AMagicBall::Destroyed()
{
	Destroy();
}

void AMagicBall::AttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 4);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 4);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
		
		const FVector SpawnLocation(Character->GetActorLocation().X, Character->GetActorLocation().Y, Character->GetActorLocation().Z + 40.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle,SpawnLocation ,GetActorRotation());
		if(Character->GetCombatState() == ECombatState::ECS_Unoccupied && !Character->GetIsInAir() && !Character->GetRolling() && !Character->GetCharacterChanging())
		{
			if(Character->GetCameraManager())
			{
				Character->GetCameraManager()->StartCameraShake(Character->GetCameraShakeHitReact(),1);
			}
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if(Character->GetCharacterState() == ECharacterState::ECS_Warrior)
			{
				UGameplayStatics::PlaySound2D(Character,Character->GetWarriorHitReactSoundCue());
				if(Character->GetFeyRendered())
				{
					AnimInstance->Montage_Play(Character->GetWarriorCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
				}
				else
				{
					AnimInstance->Montage_Play(Character->GetWarriorCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Back"));
				}
				Character->SetCombatState(ECombatState::ECS_FireTimerInProgress);
			}
			if(Character->GetCharacterState() == ECharacterState::ECS_Archer)
			{
				UGameplayStatics::PlaySound2D(Character,Character->GetArcherHitReactSoundCue());
				if(Character->GetFeyRendered())
				{
					AnimInstance->Montage_Play(Character->GetArcherCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
				}
				else
				{
					AnimInstance->Montage_Play(Character->GetArcherCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Back"));
				}
				Character->SetCombatState(ECombatState::ECS_FireTimerInProgress);
			}
		}
		Destroy();
	}
}


void AMagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character)
	{
		ClampValue =  FMath::Clamp((Alpha + 0.03f),0,1);
		Alpha = ClampValue;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
		FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
		SetActorRotation(NewRotation);
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 50.f));
	}
}

