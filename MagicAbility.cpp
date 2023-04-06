#include "MagicAbility.h"

#include "Fey.h"
#include "Grux.h"
#include "Khaimera.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AMagicAbility::AMagicAbility()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	ThunderStormCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ThunderStormCollisionCapsule"));
	ThunderStormCollisionCapsule->SetupAttachment(GetRootComponent());
	ThunderStormCollisionCapsule->SetHiddenInGame(false);
	ThunderStormCollisionCapsule->SetVisibility(true);
	

	ThunderStormParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThunderStormParticle"));
	ThunderStormParticleSystemComponent->SetupAttachment(ThunderStormCollisionCapsule);
	ThunderStormParticleSystemComponent->Activate(true);

}

void AMagicAbility::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AMagicAbility::DestroyFunction, 2.f);
	ThunderStormCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMagicAbility::MagicAbilityOverlap);
}

void AMagicAbility::DestroyFunction()
{
	Destroy();
}

void AMagicAbility::MagicAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	AFey* Fey = Cast<AFey>(OtherActor);
	if(Grux)
	{
		if(Grux->GetGruxCombatState() == EGruxCombatState::EGCS_Unoccupied)
		{
			UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(GruxHitReacts);
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Grux->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Grux->SetGruxHealth(Grux->GetGruxHealth() - 5.f);
	}
	if(Khaimera)
	{
		if(Khaimera->GetKhaimeraCombatState() == EKhaimeraCombatState::EKCS_Unoccupied)
		{
			UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(KhaimeraHitReacts);
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 5.f);
	}
	if(Fey)
	{
		if(Fey->GetFeyCombatState() == EFeyCombatState::EFCS_Unoccupied)
		{
			UAnimInstance* AnimInstance = Fey->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(FeyHitReacts);
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Fey->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Fey->SetFeyHealth(Fey->GetFeyHealth() - 5.f);
	}
}

void AMagicAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Character)
	{
		if(Character->GetThunderStormSpawned())
		{
			SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 15));
		}
	}
}

