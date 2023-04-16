


#include "SafeZoneParticle.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"


ASafeZoneParticle::ASafeZoneParticle()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SafeZoneComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SafeZoneComponent"));
	SafeZoneComponent->SetupAttachment(GetRootComponent());

	SafeZoneParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SafeZoneParticleSystemComponent"));
	SafeZoneParticleSystemComponent->SetupAttachment(SafeZoneComponent);

}


void ASafeZoneParticle::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&ASafeZoneParticle::Destroyed,2.2f);
	SafeZoneComponent->OnComponentBeginOverlap.AddDynamic(this,&ASafeZoneParticle::SafeZoneOverlap);
	SafeZoneComponent->OnComponentEndOverlap.AddDynamic(this,&ASafeZoneParticle::SafeZoneEnd);
}

void ASafeZoneParticle::SafeZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character)
	{
		Character->SetSafeZone(true);
	}
}

void ASafeZoneParticle::SafeZoneEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character)
	{
		Character->SetSafeZone(false);
	}
}

void ASafeZoneParticle::Destroyed()
{
	Destroy();
}


void ASafeZoneParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

