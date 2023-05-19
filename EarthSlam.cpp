


#include "EarthSlam.h"
#include "Khaimera.h"
#include "Grux.h"
#include "Narbash.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEarthSlam::AEarthSlam()
{

	PrimaryActorTick.bCanEverTick = true;

	EarthSlamBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("EarthSlamBoxComponent"));
	EarthSlamBoxComponent->SetupAttachment(GetRootComponent());
	
}

void AEarthSlam::BeginPlay()
{
	Super::BeginPlay();
	EarthSlamBoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AEarthSlam::EarthSlamOverlap);
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	StartCameraShake();
}

void AEarthSlam::Destroyed()
{
	if(this)
	{
		Destroy();
	}
}

void AEarthSlam::StartCameraShake()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		if(Character->GetCameraManager())
		{
			Character->GetCameraManager()->StartCameraShake(CameraShakeEarthSlam,1);
		}
	}
}

void AEarthSlam::EarthSlamOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	ANarbash* Narbash = Cast<ANarbash>(OtherActor);
	if(Grux)
	{
		if(Grux->GetGruxCombatState() == EGruxCombatState::EGCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Grux,Character->GetGruxHitReactSoundCue());
			UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Grux->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Grux->SetGruxHealth(Grux->GetGruxHealth() - 25.f);
		Destroy();
	}
	if(Khaimera)
	{
		if(Khaimera->GetKhaimeraCombatState() == EKhaimeraCombatState::EKCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Khaimera,Character->GetKhaimeraHitReactSoundCue());
			UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 30.f);
		Destroy();
	}
	if(Narbash && !Narbash->GetInvincibility())
	{
		if(Narbash->GetNarbashCombatState() == ENarbashCombatState::ENCS_Unoccupied)
		{
			UGameplayStatics::PlaySound2D(Narbash,Character->GetNarbashHitReactSoundCue());
			UAnimInstance* AnimInstance = Narbash->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Narbash->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 30.f);
		Destroy();
	}
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEarthSlam::Destroyed, 0.1f);
}

void AEarthSlam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

