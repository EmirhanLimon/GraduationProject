

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandScapeCollision.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ALandScapeCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ALandScapeCollision();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void LandScapeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LandScapeBoxComponent;
};
