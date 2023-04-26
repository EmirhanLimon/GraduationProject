


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
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Black,TEXT("23131"));
			Character->SetCharacterHealth(Character->GetCharacterHealth() - 15);
			if(Character->GetCharacterHealth() <= 0)
			{
				Character->SetCharacterHealth(0);
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

