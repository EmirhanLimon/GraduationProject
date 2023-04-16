


#include "ManaPotion.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AManaPotion::AManaPotion()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	ManaPotionStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ManaPotionStaticMesh"));
	SetRootComponent(ManaPotionStaticMeshComponent);
	ManaPotionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ManaPotionSphere"));
	ManaPotionSphereComponent->SetupAttachment(GetRootComponent());

}


void AManaPotion::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ManaPotionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AManaPotion::PickUpOverlap);
}

void AManaPotion::PickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(OtherActor == this || OtherActor == nullptr) return;
	if(Character)
	{
		Character->SetManaPotionAmount(Character->GetManaPotionAmount() + 1.f);
		Destroy();
	}
}


void AManaPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

