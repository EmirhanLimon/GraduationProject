


#include "LandScapeCollision.h"

#include "RampageStone.h"
#include "Components/BoxComponent.h"


ALandScapeCollision::ALandScapeCollision()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	LandScapeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LandScapeComponent"));
	LandScapeBoxComponent->SetupAttachment(GetRootComponent());
}


void ALandScapeCollision::BeginPlay()
{
	Super::BeginPlay();
	LandScapeBoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ALandScapeCollision::LandScapeOverlap);
}

void ALandScapeCollision::LandScapeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARampageStone* RampageStone = Cast<ARampageStone>(OtherActor);
	if(OtherActor != RampageStone) return;
	if(RampageStone)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT("assada Çarptı"));
		RampageStone->Destroyed();
	}
}

void ALandScapeCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

