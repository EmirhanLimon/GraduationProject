


#include "Arrow.h"

#include "EnemySpawnPortal.h"
#include "Grux.h"
#include "Khaimera.h"
#include "MagicTrap.h"
#include "Narbash.h"
#include "Rampage.h"
#include "NavigationSystemTypes.h"
#include "Chaos/GeometryParticlesfwd.h"
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
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green,TEXT("1231231"));
		Character->GetPickUpItem(this);
		ArrowMovementComponent->ProjectileGravityScale = ArrowMovementComponent->ProjectileGravityScale - (Character->ArrowSpeed * 0.03f);
		StartCameraShake();
		UGameplayStatics::PlaySound2D(Character,ArrowSoundCue);
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

void AArrow::StartCameraShake()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		if(Character->GetCameraManager())
		{
			Character->GetCameraManager()->StartCameraShake(CameraShakeArcherAttack,1);
		}
	}
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
			//ArrowMovementComponent->ProjectileGravityScale = 4;
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
			ClampValue =  FMath::Clamp((Alpha + 0.4f),0,1);
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
	ARampage* Rampage = Cast<ARampage>(OtherActor);
	AMagicTrap* MagicTrap = Cast<AMagicTrap>(OtherActor);
	AEnemySpawnPortal* EnemySpawnPortal = Cast<AEnemySpawnPortal>(OtherActor);
	if(OtherActor != this && OtherActor != MagicTrap && OtherActor != EnemySpawnPortal)
	{
		if(Grux && !Grux->GetDied())
		{
			if(Character->GetChangeCharacterFormValue() <= 99)
			{
				Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
			}

			if(Character->GetSwitchCounter() == 1)
			{
				Grux->SetGruxHealth(Grux->GetGruxHealth() - (20.f + (Character->ArrowDamageWithSpeed / 10)));
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
				const float HitReactChance = FMath::FRandRange(0.f,1.f);
				if(HitReactChance <= 0.75f)
				{
					UGameplayStatics::PlaySound2D(Grux,Character->GetGruxHitReactSoundCue());
					UAnimInstance* AnimInstance = Grux->GetMesh()->GetAnimInstance();
					AnimInstance->Montage_Play(Grux->GetGruxHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
					Grux->SetGruxCombatState(EGruxCombatState::EGCS_FireTimerInProgress);
					Grux->GetCharacterMovement()->MaxWalkSpeed = 0;
				}
			}
			if(Grux->GetPawnSensingComponent())
			{
				Grux->GetPawnSensingComponent()->HearingThreshold = 10000;
				Grux->GetPawnSensingComponent()->LOSHearingThreshold = 11000;
			}
			if(Character->GetBloodFXParticle())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Character->GetBloodFXParticle(),Grux->GetActorLocation() + (Grux->GetActorForwardVector() * 50.f),FRotator(0,0,0));
			}
		}
		if(Khaimera && !Khaimera->GetDied())
		{
			if(Character->GetChangeCharacterFormValue() <= 99)
			{
				Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
			}
			if(Character->GetSwitchCounter() == 1)
			{
				Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - (25.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - 30.f);
			}
			else
			{
				Khaimera->SetKhaimeraHealth(Khaimera->GetKhaimeraHealth() - (10.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			if(Khaimera->GetKhaimeraCombatState() == EKhaimeraCombatState::EKCS_Unoccupied)
			{
				const float HitReactChance = FMath::FRandRange(0.f,1.f);
				if(HitReactChance <= 0.75f)
				{
					UGameplayStatics::PlaySound2D(Khaimera,Character->GetKhaimeraHitReactSoundCue());
					UAnimInstance* AnimInstance = Khaimera->GetMesh()->GetAnimInstance();
					AnimInstance->Montage_Play(Khaimera->GetKhaimeraHitReacts());
					AnimInstance->Montage_JumpToSection(FName("Front"));
					Khaimera->SetKhaimeraCombatState(EKhaimeraCombatState::EKCS_FireTimerInProgress);
					Khaimera->GetCharacterMovement()->MaxWalkSpeed = 0;
				}
			}
			if(Khaimera->GetPawnSensingComponent())
			{
				Khaimera->GetPawnSensingComponent()->HearingThreshold = 10000;
				Khaimera->GetPawnSensingComponent()->LOSHearingThreshold = 11000;
			}
			if(Character->GetBloodFXParticle())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Character->GetBloodFXParticle(),Khaimera->GetActorLocation() + (Khaimera->GetActorForwardVector() * 50.f),FRotator(0,0,0));
			}
		}
		if(Narbash && !Narbash->GetDied() && !Narbash->GetInvincibility())
		{
			if(Character->GetChangeCharacterFormValue() <= 99)
			{
				Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
			}
			if(Character->GetSwitchCounter() == 1)
			{
				Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - (25.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - 30.f);
			}
			else
			{
				Narbash->SetNarbashHealth(Narbash->GetNarbashHealth() - (10.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			if(Narbash->GetNarbashCombatState() == ENarbashCombatState::ENCS_Unoccupied)
			{
				const float HitReactChance = FMath::FRandRange(0.f,1.f);
				if(HitReactChance <= 0.25f)
				{
					UGameplayStatics::PlaySound2D(Narbash,Character->GetNarbashHitReactSoundCue());
					UAnimInstance* AnimInstance = Narbash->GetMesh()->GetAnimInstance();
					AnimInstance->Montage_Play(Narbash->GetHitReactsAnimMontage());
					AnimInstance->Montage_JumpToSection(FName("Front"));
					Narbash->SetNarbashCombatState(ENarbashCombatState::ENCS_FireTimerInProgress);
					Narbash->GetCharacterMovement()->MaxWalkSpeed = 0;
				}
			}
			if(Narbash->GetPawnSensingComponent())
			{
				Narbash->GetPawnSensingComponent()->HearingThreshold = 10000;
				Narbash->GetPawnSensingComponent()->LOSHearingThreshold = 11000;
			}
			if(Character->GetBloodFXParticle())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Character->GetBloodFXParticle(),Narbash->GetActorLocation() + (Narbash->GetActorForwardVector() * 50.f),FRotator(0,0,0));
			}
		}
		if(Rampage && !Rampage->GetRampageDied())
		{
			if(Character->GetChangeCharacterFormValue() <= 99)
			{
				Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
			}
			if(Character->GetSwitchCounter() == 1)
			{
				Rampage->SetRampageHealth(Rampage->GetRampageHealth() - (25.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Rampage->SetRampageHealth(Rampage->GetRampageHealth() - 30.f);
			}
			else
			{
				Rampage->SetRampageHealth(Rampage->GetRampageHealth() - (10.f + (Character->ArrowDamageWithSpeed / 10)));
			}
		}
		if(Fey && !Fey->GetDied())
		{
			if(Character->GetChangeCharacterFormValue() <= 99)
			{
				Character->SetChangeCharacterFormValue(Character->GetChangeCharacterFormValue() + 1.f);
			}
			if(Character->GetSwitchCounter() == 1)
			{
				Fey->SetFeyHealth(Fey->GetFeyHealth() - (25.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			else if(Character->GetSwitchCounter() == 2)
			{
				Fey->SetFeyHealth(Fey->GetFeyHealth() - 30.f);
			}
			else
			{
				Fey->SetFeyHealth(Fey->GetFeyHealth() - (10.f + (Character->ArrowDamageWithSpeed / 10)));
			}
			if(Fey->GetFeyCombatState() == EFeyCombatState::EFCS_Unoccupied)
			{
				UGameplayStatics::PlaySound2D(Fey,Character->GetFeyHitReactSoundCue());
				UAnimInstance* AnimInstance = Fey->GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Fey->GetFeyHitReacts());
				AnimInstance->Montage_JumpToSection(FName("Front"));
				Fey->SetFeyCombatState(EFeyCombatState::EFCS_FireTimerInProgress);
				Fey->GetCharacterMovement()->MaxWalkSpeed = 0;
			}
			if(Fey->GetPawnSensingComponent())
			{
				Fey->GetPawnSensingComponent()->HearingThreshold = 10000;
				Fey->GetPawnSensingComponent()->LOSHearingThreshold = 11000;
			}
			if(Character->GetBloodFXParticle())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Character->GetBloodFXParticle(),Fey->GetActorLocation() + (Fey->GetActorForwardVector() * 50.f),FRotator(0,0,0));
			}
		}
		
		ArrowMovementComponent->StopMovementImmediately();
		ArrowMovementComponent->ProjectileGravityScale = 0;
		if(ArrowDestroyParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ArrowDestroyParticle, GetActorLocation());
		}
		if(Character->GetSwitchCounter() == 1 || Character->GetSwitchCounter() == 2)
		{
			Character->SetSwitchCounter(0);
		}
		Destroy();
		ClampValue = 0;
		Alpha = 0;
		Character->ArrowDamageWithSpeed = 0;
	}
}



