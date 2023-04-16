


#include "Arrow.h"

#include "Grux.h"
#include "Khaimera.h"
#include "Narbash.h"
#include "NavigationSystemTypes.h"
#include "Engine/LODActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


AArrow::AArrow() :
	ClampValue(0.f),
	Alpha(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	ArrowMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowProjectileMovementComponent"));
	ArrowBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ArrowBoxComponent"));
	ArrowBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArrowCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ArrowCollsionComponent"));
	ArrowCollisionComponent->SetupAttachment(ArrowBoxComponent);
	ArrowMovementComponent->ProjectileGravityScale = 4;

	FireArrowParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireArrowParticleComponent"));
	FireArrowParticle->SetupAttachment(ArrowBoxComponent);
	FireArrowParticle->SetVisibility(false);
	

	UltimateArrowParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("UltimateArrowParticleComponent"));
	UltimateArrowParticle->SetupAttachment(ArrowBoxComponent);
	UltimateArrowParticle->SetVisibility(false);

}
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ArrowCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AArrow::BeginOverlap);
	if(Character)
	{
		Character->GetPickUpItem(this);
		ArrowMovementComponent->ProjectileGravityScale = ArrowMovementComponent->ProjectileGravityScale - (Character->ArrowSpeed * 0.03f);
	}
	/*TSubclassOf<AGrux> classToFind;
	classToFind = AGrux::StaticClass();
	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),classToFind,foundEnemies);
	auto GruxRef = foundEnemies[0];
	AGrux* GruxRefa = Cast<AGrux>(GruxRef);
	if(GruxRefa)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Blue,TEXT("2131231"));
	}*/
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
		switch (Character->GetSwitchCounter())
		{
		case 0:
			FireArrowParticle->Deactivate();
			UltimateArrowParticle->Deactivate();
			ArrowMovementComponent->InitialSpeed = 5000;
			ArrowMovementComponent->MaxSpeed = 5000;
			FireArrowParticle->SetVisibility(false);
			UltimateArrowParticle->SetVisibility(false);
			ArrowMovementComponent->ProjectileGravityScale = 4;
			break;
		case 1:
			FireArrowParticle->Activate(true);
			FireArrowParticle->SetVisibility(true);
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT("qweqwwe"));
			break;
		case 2:
			UltimateArrowParticle->Activate(true);
			UltimateArrowParticle->SetVisibility(true);
			ArrowMovementComponent->InitialSpeed = 3000;
			ArrowMovementComponent->MaxSpeed = 3000;
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Black,TEXT("qweqwwe"));
			break;
		case 3:
			ClampValue =  FMath::Clamp((Alpha + 0.2f),0,1);
			Alpha = ClampValue;
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Character->GetEnemyTargetLocation());
			FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
			SetActorRotation(NewRotation);
			SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 10.f));
			ArrowMovementComponent->ProjectileGravityScale = 0;
		}
}

void AArrow::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGrux* Grux = Cast<AGrux>(OtherActor);
	AKhaimera* Khaimera = Cast<AKhaimera>(OtherActor);
	AFey* Fey = Cast<AFey>(OtherActor);
	ANarbash* Narbash = Cast<ANarbash>(OtherActor);
	if(OtherActor != this)
	{
		if(Grux && !Grux->GetDied())
		{
			if(Character->GetSwitchCounter() == 1)
			{
				Grux->SetGruxHealth(Grux->GetGruxHealth() - (20.f + (Character->ArrowSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Grux->SetGruxHealth(Grux->GetGruxHealth() - 30.f);
			}
			else
			{
				Grux->SetGruxHealth(Grux->GetGruxHealth() - (10.f + (Character->ArrowSpeed / 10)));
			}
			if(Grux->GetGruxCombatState() == EGruxCombatState::EGCS_Unoccupied)
			{
				UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Grux->SetGruxCombatState(EGruxCombatState::EGCS_FireTimerInProgress);
				Grux->GetCharacterMovement()->MaxWalkSpeed = 0;
			}
		}
		if(Khaimera && !Khaimera->GetDied())
		{
			if(Character->GetSwitchCounter() == 1)
			{
				Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - (25.f + (Character->ArrowSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 30.f);
			}
			else
			{
				Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - (10.f + (Character->ArrowSpeed / 10)));
			}
			if(Khaimera->GetKhaimeraCombatState() == EKhaimeraCombatState::EKCS_Unoccupied)
			{
				UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Khaimera->SetKhaimeraCombatState(EKhaimeraCombatState::EKCS_FireTimerInProgress);
				Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
			}
		}
		if(Narbash && !Narbash->GetDied() && !Narbash->GetInvincibility())
		{
			if(Character->GetSwitchCounter() == 1)
			{
				Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - (25.f + (Character->ArrowSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 30.f);
			}
			else
			{
				Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - (10.f + (Character->ArrowSpeed / 10)));
			}
			if(Narbash->GetNarbashCombatState() == ENarbashCombatState::ENCS_Unoccupied)
			{
				UAnimInstance* AnimInstance = Narbash->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Narbash->SetNarbashCombatState(ENarbashCombatState::ENCS_FireTimerInProgress);
				Narbash->GetCharacterMovement()->MaxWalkSpeed = 0;
			}
		}
		if(Fey && !Fey->GetDied())
		{
			if(Character->GetSwitchCounter() == 1)
			{
				Fey->SetFeyHealth(Fey->GetFeyHealth() - (25.f + (Character->ArrowSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Fey->SetFeyHealth(Fey->GetFeyHealth() - 30.f);
			}
			else
			{
				Fey->SetFeyHealth(Fey->GetFeyHealth() - (10.f + (Character->ArrowSpeed / 10)));
			}
			if(Fey->GetFeyCombatState() == EFeyCombatState::EFCS_Unoccupied)
			{
				UAnimInstance* AnimInstance = Fey->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Fey->GetFeyHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Fey->SetFeyCombatState(EFeyCombatState::EFCS_FireTimerInProgress);
				Fey->GetCharacterMovement()->MaxWalkSpeed = 0;
			}
		}
		
		ArrowMovementComponent->StopMovementImmediately();
		ArrowMovementComponent->ProjectileGravityScale = 0;
		if(ArrowDestroyParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ArrowDestroyParticle, GetActorLocation());
		}
		//Character->SetReinforcedArrowUsing(false);
		if(Character->GetSwitchCounter() == 1 || Character->GetSwitchCounter() == 2)
		{
			Character->SetSwitchCounter(0);
		}
		Destroy();
		ClampValue = 0;
		Alpha = 0;
	}
}



