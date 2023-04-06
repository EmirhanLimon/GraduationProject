


#include "EarthSlam.h"
#include "Khaimera.h"
#include "Grux.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
}

void AEarthSlam::Destroyed()
{
	if(this)
	{
		Destroy();
	}
}

void AEarthSlam::EarthSlamOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	if(Grux)
	{
		if(Grux->GetGruxCombatState() == EGruxCombatState::EGCS_Unoccupied)
		{
			UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(GruxHitReacts);
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
			UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(KhaimeraHitReacts);
			AnimInstance->Montage_JumpToSection(FName("Front"));
			Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
		}
		Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 30.f);
		Destroy();
	}
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEarthSlam::Destroyed, 0.1f);
}

void AEarthSlam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

