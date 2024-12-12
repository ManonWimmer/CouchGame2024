// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventZonesV2Manager.generated.h"

class APillarElement;

UENUM(BlueprintType)
enum class EPillarManagerState : uint8
{
	Waiting,
	Static
};

class APillarZone;

UCLASS()
class COUCHGAME2024_API AEventZonesV2Manager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventZonesV2Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Bind();

	UPROPERTY()
	bool bHasBeenBind = false;

	UFUNCTION()
	void SetStartValues();

	UFUNCTION()
	void GetPhase2ZonesAndPillars();

	UFUNCTION()
	void GetRandomPillars();

	UFUNCTION()
	void ShowRandomPillars();

	UFUNCTION()
	void ResetRandomPillars();

	UPROPERTY()
	TArray<TObjectPtr<APillarZone>> Phase2Zones;

	UPROPERTY()
	TArray<TObjectPtr<APillarElement>> Phase2Pillars;

	UPROPERTY()
	TMap<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>> SelectedZonesAndPillars;
	
	TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>> TrickedZoneAndPillar1;
	
	TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>> TrickedZoneAndPillar2;

	UFUNCTION()
	void StartPhase2();

	UFUNCTION()
	void EndPhase2();

	UPROPERTY()
	bool bIsInPhase2 = false;

	UPROPERTY()
	EPillarManagerState CurrentState = EPillarManagerState::Waiting;

	UPROPERTY()
	float CurrentWaitTime = 0.f;

	UPROPERTY()
	float TimeWaited = 0.f;

	UPROPERTY()
	float StartedTime = 0.f;

	UFUNCTION()
	void Wait(float DeltaTime);

	UFUNCTION()
	void Static(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MinTimeWaitingOnPillar = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MaxTimeWaitingOnPillar = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MinTimeStaticOnPillar = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MaxTimeStaticOnPillar = 15.f;
};
