// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillarZone.generated.h"

class APillarElement;
class USphereComponent;

UENUM(BlueprintType)
enum class EPillarZoneState : uint8
{
	Waiting,
	Moving
};

UCLASS()
class COUCHGAME2024_API APillarZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APillarZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
					  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
	void GetLevelPillars(); // to get every round, not just at begin play
	
	UFUNCTION()
	void SwitchPillar();
	
	UFUNCTION()
	void GetRandomPillar();

	UFUNCTION()
	void MoveToPillar();

	UFUNCTION()
	void Wait();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY()
	TArray<APillarElement*> ScenePillarZones;

	UPROPERTY()
	TObjectPtr<APillarElement> TargetPillar = nullptr;
	
};
