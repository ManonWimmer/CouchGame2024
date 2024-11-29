// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "PinballElements/PinballElement.h"
#include "MoleElement.generated.h"

UCLASS()
class COUCHGAME2024_API AMoleElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMoleElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TriggerElementWithPlayer(APlayerBall* InPlayerBall) override;
	
	virtual EPinballElementID GetElementID() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoleCollected);

	UPROPERTY()
	FMoleCollected OnMoleCollected;

	UFUNCTION()
	void SpawnMole();

	UFUNCTION()
	void DespawnMole();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnImpactedByPlayer"))
	void ReceiveImpactedByPlayer();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSpawnMole"))
	void ReceiveOnSpawnMole();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDespawnMole"))
	void ReceiveOnDespawnMole();
	
	UPROPERTY()
	int MoleToPoints = 200;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UCurveFloat* SpawnAnimationCurve;

	UPROPERTY()
	UTimelineComponent* SpawnTimeline;

	UPROPERTY()
	FOnTimelineFloat InterpFunction{};

	UPROPERTY()
	FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
	void HandleTimelineProgress(float Value);

	UFUNCTION()
	void OnTimelineFinished();

	UPROPERTY()
	float InitialZ = 0.f;

	UPROPERTY()
	float TargetZ = 0.f;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float Height = 300.f;
	
private:

	UFUNCTION()
	void BumpEffect();

	UPROPERTY()
	bool bHasBeenHitByPlayer = false;
};
