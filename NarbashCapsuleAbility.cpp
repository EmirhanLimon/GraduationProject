
#include "NarbashCapsuleAbility.h"

#include "WarriorCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

ANarbashCapsuleAbility::ANarbashCapsuleAbility() :
	Collision(false)
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MagicCapsule"));
	MagicCapsule->SetupAttachment(GetRootComponent());
	
	MagicCapsuleParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicCapsuleParticleSystemComponent"));
	MagicCapsuleParticleSystemComponent->SetupAttachment(MagicCapsule);

	MagicUndulationParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicUndulationParticleSystemComponent"));
	MagicUndulationParticleSystemComponent->SetupAttachment(MagicCapsule);
}


void ANarbashCapsuleAbility::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&ANarbashCapsuleAbility::Destroyed,0.7f);
	MagicCapsule->OnComponentBeginOverlap.AddDynamic(this,&ANarbashCapsuleAbility::CapsuleAbilityOverlap);
}

void ANarbashCapsuleAbility::CapsuleAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarriorCharacter* Character = Cast<AWarriorCharacter>(OtherActor);
	if(OtherActor == nullptr || OtherActor == this) return;
	if(Character && !Collision && !Character->GetSafeZone())
	{
		Collision = true;
		if(!Character->GetInvincibility())
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 3.f);
			
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
			}
		}
		else
		{
			Character->SetCharacterHealth(Character->GetCharacterHealth() + 3.f);
			if(Character->GetCharacterHealth() >= 100)
			{
				Character->SetCharacterHealth(100);
			}
		}

	}
}

void ANarbashCapsuleAbility::Destroyed()
{
	Destroy();
}

void ANarbashCapsuleAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

