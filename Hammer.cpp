


#include "Hammer.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


AHammer::AHammer()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	HammerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HammerSphere"));
	HammerSphere->SetupAttachment(GetRootComponent());

	HammerParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HammerParticleSystemComponent"));
	HammerParticleSystemComponent->SetupAttachment(HammerSphere);
}


void AHammer::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Character)
	{
		TargetLocation = Character->GetActorLocation();
	}
	HammerSphere->OnComponentBeginOverlap.AddDynamic(this, &AHammer::HammerOverlap);
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&AHammer::Destroyed,2.f);
}

void AHammer::HammerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(OtherActor == this || OtherActor == nullptr) return;
	if(Character)
	{
		const FVector SpawnEmitterLocation(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z + 60.f);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HammerHitParticleSystem,SpawnEmitterLocation ,GetActorRotation());
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 10);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 10);
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
				if(!Character->GetNarbashRendered())
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
				if(!Character->GetNarbashRendered())
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
		Destroyed();
	}
}

void AHammer::Destroyed()
{
	Destroy();
}


void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 10.f));
	}
}
