


#include "RampageStone.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ARampageStone::ARampageStone() :
	Distance(0.f),
	FirstTargetLocation(0,0,0),
	ClampValue(0.f),
	Alpha(0.f),
	Simulate(false)
{

	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	RampageStoneStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RampageStoneStaticMesh"));
	RampageStoneStaticMeshComponent->SetupAttachment(GetRootComponent());
	RampageStoneBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RampageStoneBoxComponent"));
	RampageStoneBoxComponent->SetupAttachment(RampageStoneStaticMeshComponent);
	RampageStoneBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RampageStoneStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}


void ARampageStone::BeginPlay()
{
	Super::BeginPlay();
	
	RampageStoneStaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(Character)
	{
		Distance = FVector::Distance(GetActorLocation(),Character->GetActorLocation());
		FirstTargetLocation = FVector(Character->GetActorLocation().X,Character->GetActorLocation().Y,Distance * 20.f);
		TargetLocation = FVector(0,0,FirstTargetLocation.Z);
	}
	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer,this,&ARampageStone::CollisionResponse,0.2f);
}

void ARampageStone::Destroyed()
{
	if(!IsValid(Character))
	{
		Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	else
	{
		if(Distance < 750)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RockHitWorldParticle,FVector(Character->GetActorLocation().X,Character->GetActorLocation().Y,Character->GetActorLocation().Z),GetActorRotation(),FVector(5,5,2));
			if(!Character->GetInvincibility())
			{
				Character->SetCharacterHealth(Character->GetCharacterHealth() - 20);
				if(Character->GetCharacterHealth() <= 0)
				{
					Character->SetCharacterHealth(0);
				}
			}
			else
			{
				Character->SetCharacterHealth(Character->GetCharacterHealth() + 20);
				if(Character->GetCharacterHealth() >= 100)
				{
					Character->SetCharacterHealth(100);
				}
			}	
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RockHitWorldParticle,FVector(GetActorLocation().X,GetActorLocation().Y,Character->GetActorLocation().Z),GetActorRotation(),FVector(5,5,2));
		}	
	}
	if(Character->GetCameraManager() && CameraShakeHitStone)
	{
		Character->GetCameraManager()->StartCameraShake(CameraShakeHitStone,1);
	}
	Destroy();
}

void ARampageStone::CollisionResponse()
{
	RampageStoneBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

void ARampageStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!Simulate && Character && this)
	{
		ClampValue =  FMath::Clamp((Alpha + 0.2f),0,1);
		Alpha = ClampValue;
		TargetLocation = FMath::VInterpTo(FVector(0,0,TargetLocation.Z),FVector(0,0,50) , DeltaTime, 10.f);
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),FVector(FirstTargetLocation.X,FirstTargetLocation.Y,TargetLocation.Z) + Character->GetActorForwardVector());
		FRotator NewRotation = FRotator(FQuat::Slerp(UE::Math::TQuat<double>(GetActorRotation()), UE::Math::TQuat<double>(TargetRotation), Alpha));
		SetActorRotation(NewRotation);
		SetActorLocation(GetActorLocation() + ((GetActorForwardVector() * 10000.f) * DeltaTime));
	}
	if(Character)
	{
		Distance = FVector::Distance(GetActorLocation(),Character->GetActorLocation());
	}

	if(Distance < 500)
	{
		RampageStoneStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		CollisionResponse();
		RampageStoneStaticMeshComponent->SetSimulatePhysics(true);
		Simulate = true;
	}
	
}

