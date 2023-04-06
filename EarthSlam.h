

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EarthSlam.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AEarthSlam : public AActor
{
	GENERATED_BODY()
	
public:	

	AEarthSlam();

protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void EarthSlamOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	

	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* EarthSlamBoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* GruxHitReacts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KhaimeraHitReacts;
};
