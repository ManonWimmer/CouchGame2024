// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillarZone.generated.h"

class USpotLightComponent;
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
	void GetRandomPillar();

	UFUNCTION()
	void MoveToPillar(float DeltaTime);

	UFUNCTION()
	void Wait(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY()
	TArray<APillarElement*> ScenePillars;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APillarElement> TargetPillar = nullptr;

	UPROPERTY()
	EPillarZoneState CurrentZoneState = EPillarZoneState::Waiting;
	
	UPROPERTY()
	float CurrentWaitTime = 0.f;

	UPROPERTY()
	float TimeWaited = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TObjectPtr<APillarElement> SpawnPillar = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MinTimeWaitingOnPillar = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MaxTimeWaitingOnPillar = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MoveSpeed = 500.f;

	UFUNCTION()
	void OnStartPhase1();

	UFUNCTION()
	void OnEndPhase1AndStartPhase2();

	UFUNCTION()
	void OnEndPhase2();

	UPROPERTY()
	bool bIsInPhase1 = false;

	UPROPERTY()
	bool bIsInPhase2 = false;

	UFUNCTION()
	void Bind();

	UPROPERTY()
	bool bHasBeenBind = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
	TObjectPtr<USpotLightComponent> SpotLightComponent;

	UFUNCTION()
	void ShowZone() const;

	UFUNCTION()
	void HideZone() const;
};
