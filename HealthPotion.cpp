


#include "HealthPotion.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AHealthPotion::AHealthPotion()
{

	PrimaryActorTick.bCanEverTick = true;

	HealthPotionStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthPotionStaticMesh"));
	SetRootComponent(HealthPotionStaticMeshComponent);
	HealthPotionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HealthPotionSphere"));
	HealthPotionSphereComponent->SetupAttachment(GetRootComponent());
}


void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	HealthPotionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHealthPotion::PickUpOverlap);
}

void AHealthPotion::PickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(OtherActor == this || OtherActor == nullptr) return;
	if(Character)
	{
		Character->SetHealthPotionAmount(Character->GetHealthPotionAmount() + 1.f);
		Destroy();
	}
}

void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

