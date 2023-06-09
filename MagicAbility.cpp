#include "MagicAbility.h"

#include "Fey.h"
#include "Grux.h"
#include "Khaimera.h"
#include "Narbash.h"
#include "Rampage.h"
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
	StartCameraShake();
}

void AMagicAbility::DestroyFunction()
{
	Destroy();
}

void AMagicAbility::StartCameraShake()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		if(Character->GetCameraManager())
		{
			Character->GetCameraManager()->StartCameraShake(CameraShakeThunderStorm,1);
		}
	}
}

void AMagicAbility::MagicAbilityOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	AFey* Fey = Cast<AFey>(OtherActor);
	ANarbash* Narbash = Cast<ANarbash>(OtherActor);
	ARampage* Rampage = Cast<ARampage>(OtherActor);
	if(Grux)
	{
		if(Character->GetChangeCharacterFormValue() <= 99)
		{
			Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
		}
		if(Grux->GetGruxCombatState() == EGruxCombatState::EGCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Grux,Character->GetGruxHitReactSoundCue());
			UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Grux->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Grux->SetGruxHealth(Grux->GetGruxHealth() - 5.f);
	}
	if(Khaimera)
	{
		if(Character->GetChangeCharacterFormValue() <= 99)
		{
			Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
		}
		if(Khaimera->GetKhaimeraCombatState() == EKhaimeraCombatState::EKCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Khaimera,Character->GetKhaimeraHitReactSoundCue());
			UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 5.f);
	}
	if(Fey)
	{
		if(Character->GetChangeCharacterFormValue() <= 99)
		{
			Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
		}
		if(Fey->GetFeyCombatState() == EFeyCombatState::EFCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Fey,Character->GetFeyHitReactSoundCue());
			UAnimInstance* AnimInstance = Fey->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Fey->GetFeyHitReacts());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Fey->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Fey->SetFeyHealth(Fey->GetFeyHealth() - 5.f);
	}
	if(Narbash && !Narbash->GetInvincibility())
	{
		if(Character->GetChangeCharacterFormValue() <= 99)
		{
			Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
		}
		if(Narbash->GetNarbashCombatState() == ENarbashCombatState::ENCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Narbash,Character->GetNarbashHitReactSoundCue());
			UAnimInstance* AnimInstance = Narbash->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Narbash->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 5.f);
	}
	if(Rampage)
	{
		if(Character->GetChangeCharacterFormValue() <= 99)
		{
			Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
		}
		Rampage->SetRampageHealth(Rampage->GetRampageHealth() - 5.f);
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

