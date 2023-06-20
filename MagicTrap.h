

#pragma once

#include "CoreMinimal.h"
#include "Grux.h"
#include "Khaimera.h"
#include "Narbash.h"
#include "Rampage.h"
#include "GameFramework/Actor.h"
#include "WarriorCharacter.h"
#include "MagicTrap.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AMagicTrap : public AActor
{
	GENERATED_BODY()
	
public:	

	AMagicTrap();

protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void TrapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* MagicTrapComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* MagicTrapParticleSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess))
	AGrux* Grux;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess))
	AKhaimera* Khaimera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess))
	ANarbash* Narbash;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess))
	ARampage* Rampage;
};
