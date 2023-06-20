


#include "EnemySpawnPortal.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemySpawnPortal::AEnemySpawnPortal() :
	bRotation(false)
{

	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	StageOneStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageOneStaticMesh"));
	StageOneStaticMeshComponent->SetupAttachment(GetRootComponent());
	StageTwoStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageTwoStaticMesh"));
	StageTwoStaticMeshComponent->SetupAttachment(GetRootComponent());
	StageThreeStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageThreeStaticMesh"));
	StageThreeStaticMeshComponent->SetupAttachment(GetRootComponent());
	StageFourStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageFourStaticMesh"));
	StageFourStaticMeshComponent->SetupAttachment(GetRootComponent());
	StageFiveStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StageFiveStaticMesh"));
	StageFiveStaticMeshComponent->SetupAttachment(GetRootComponent());
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
}


void AEnemySpawnPortal::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&AEnemySpawnPortal::PortalOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this,&AEnemySpawnPortal::PortalEnd);
}

void AEnemySpawnPortal::PortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character == nullptr) return;
	if(Character && Character->GetAmountOfDeadEnemies() == 5)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::White,TEXT("OLDU"));
		bRotation = true;
		FTimerHandle RotationTimer;
		GetWorldTimerManager().SetTimer(RotationTimer,this,&AEnemySpawnPortal::RotationEnd,2.f);
		Character->SetAmountOfDeadEnemies(0);
		Character->SetEnemySpawning(true);
		UGameplayStatics::PlaySound2D(this,RockFrictionSoundCue);
		switch (Character->GetWaveState())
		{
		case EWaveState::EWS_WaveOne:
			StaticMeshComponentVeriable = StageOneStaticMeshComponent;
			break;
		case EWaveState::EWS_WaveTwo:
			StaticMeshComponentVeriable = StageTwoStaticMeshComponent;
			break;
		case EWaveState::EWS_WaveThree:
			StaticMeshComponentVeriable = StageThreeStaticMeshComponent;
			break;
		case EWaveState::EWS_WaveFour:
			StaticMeshComponentVeriable = StageFourStaticMeshComponent;
			break;
		case EWaveState::EWS_WaveFive:
			StaticMeshComponentVeriable = StageFiveStaticMeshComponent;
			break;
		}
	}
}

void AEnemySpawnPortal::PortalEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<AWarriorCharacter>(OtherActor);
	if(Character == nullptr) return;
	if(Character)
	{
		Character->SetEnemySpawning(false);
	}
}

void AEnemySpawnPortal::RotateStaticMesh(float DeltaTime)
{
	FRotator NewRotation(StaticMeshComponentVeriable->GetRelativeRotation().Pitch,StaticMeshComponentVeriable->GetRelativeRotation().Yaw,StaticMeshComponentVeriable->GetRelativeRotation().Roll);
	StaticMeshComponentVeriable->SetRelativeRotation(FRotator(NewRotation.Pitch + (15.f * DeltaTime),NewRotation.Yaw,NewRotation.Roll));
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Red,TEXT("2131"));
}

void AEnemySpawnPortal::RotationEnd()
{
	bRotation = false;
}


void AEnemySpawnPortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bRotation)
	{
		RotateStaticMesh(DeltaTime);
	}

}

