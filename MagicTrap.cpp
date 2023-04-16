// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicTrap.h"


#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AMagicTrap::AMagicTrap()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	MagicTrapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MagicTrapComponent"));
	MagicTrapComponent->SetupAttachment(RootComponent);
	MagicTrapComponent->SetHiddenInGame(false);
	MagicTrapComponent->SetVisibility(true);

	MagicTrapParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MagicTrapParticleComponent"));
	MagicTrapParticleSystemComponent->SetupAttachment(MagicTrapComponent);
	MagicTrapParticleSystemComponent->Activate(true);
}

void AMagicTrap::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AMagicTrap::Destroyed, 5.f);
	MagicTrapComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicTrap::TrapOverlap);
	MagicTrapComponent->OnComponentEndOverlap.AddDynamic(this, &AMagicTrap::TrapEnd);
}

void AMagicTrap::Destroyed()
{
	Super::Destroyed();
	Destroy();
}

void AMagicTrap::TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == nullptr) return;
	Grux = Cast<AGrux>(OtherActor);
	Khaimera = Cast<AKhaimera>(OtherActor);
	Narbash = Cast<ANarbash>(OtherActor);
	if(Grux)
	{
		Grux->SetGruxStunned(true);
		Grux->GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	if(Khaimera)
	{
		Khaimera->SetKhaimeraStunned(true);
		Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
	}
	if(Narbash)
	{
		Narbash->SetNarbashStunned(true);
		Narbash->GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void AMagicTrap::TrapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if(OtherActor == nullptr) return;
	Grux = Cast<AGrux>(OtherActor);
	Khaimera = Cast<AKhaimera>(OtherActor);
	Narbash = Cast<ANarbash>(OtherActor);
	if(Grux)
	{
		Grux->SetGruxStunned(false);
		Grux->GetCharacterMovement()->MaxWalkSpeed = 600;
	}
	if(Khaimera)
	{
		Khaimera->SetKhaimeraStunned(false);
		Khaimera->GetCharacterMovement()->MaxWalkSpeed = 800;
	}
	if(Narbash)
	{
		Narbash->SetNarbashStunned(false);
		Narbash->GetCharacterMovement()->MaxWalkSpeed = 800;
	}
}

void AMagicTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

