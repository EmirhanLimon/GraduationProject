


#include "GroundSmash.h"

#include "WarriorCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AGroundSmash::AGroundSmash() :
	bCanAttack(true)
{

	PrimaryActorTick.bCanEverTick = true;
	

	GroundSmashComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GroundSmashComponent"));
	GroundSmashComponent->SetupAttachment(GetRootComponent());
	HandImpactParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GroundSmashParticle"));
	HandImpactParticleSystemComponent->SetupAttachment(GroundSmashComponent);

}

void AGroundSmash::BeginPlay()
{
	Super::BeginPlay();
	GroundSmashComponent->OnComponentBeginOverlap.AddDynamic(this,&AGroundSmash::GroundSmashOverlap);
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&AGroundSmash::Destroyed,0.5f);
	PlayCameraShake();
	UGameplayStatics::PlaySound2D(this,Character->GetStoneHitWorldSoundCue());
}

void AGroundSmash::PlayCameraShake()
{
	if(Character && Character->GetCameraManager())
	{
		Character->GetCameraManager()->StartCameraShake(CameraShakeGroundSmash,1.f);
	}
}

void AGroundSmash::Destroyed()
{
	Destroy();
}

void AGroundSmash::GroundSmashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character == nullptr) return;
	if(Character && bCanAttack)
	{
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 15);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if(Character->GetCombatState() == ECombatState::ECS_Unoccupied && !Character->GetIsInAir() && !Character->GetRolling() && !Character->GetCharacterChanging())
			{
				if(Character->GetCharacterState() == ECharacterState::ECS_Warrior)
				{
					UGameplayStatics::PlaySound2D(Character,Character->GetWarriorHitReactSoundCue());
					AnimInstance->Montage_Play(Character->GetWarriorCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
					Character->SetCombatState(ECombatState::ECS_FireTimerInProgress);
				}
				if(Character->GetCharacterState() == ECharacterState::ECS_Archer)
				{
					UGameplayStatics::PlaySound2D(Character,Character->GetArcherHitReactSoundCue());
					AnimInstance->Montage_Play(Character->GetArcherCharacterHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
					Character->SetCombatState(ECombatState::ECS_FireTimerInProgress);
				}
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 15);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}
		bCanAttack = false;
	}
}

void AGroundSmash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

