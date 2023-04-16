

#pragma once

#include "CoreMinimal.h"
#include "WarriorCharacter.h"
#include "GameFramework/Actor.h"
#include "ManaPotion.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AManaPotion : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AManaPotion();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void PickUpOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ManaPotionSphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ManaPotionStaticMeshComponent;
};
