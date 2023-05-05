

#pragma once

#include "CoreMinimal.h"
#include "WarriorCharacter.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPortal.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AEnemySpawnPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AEnemySpawnPortal();

protected:
	
	virtual void BeginPlay() override;
	UFUNCTION()
	void PortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void PortalEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void RotateStaticMesh(float DeltaTime);
	void RotationEnd();
public:	
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess))
	AWarriorCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StageOneStaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StageTwoStaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StageThreeStaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StageFourStaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StageFiveStaticMeshComponent;
	UStaticMeshComponent* StaticMeshComponentVeriable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bRotation;
};




