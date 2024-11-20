// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "PowerUp/PowerUp.h"
#include "PowerUpMole.generated.h"

UCLASS()
class COUCHGAME2024_API APowerUpMole : public APowerUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpMole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual EPowerUpID GetPowerUpID() const override;
    
	virtual void TriggerPowerUp(int PlayerIndex) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoleCollected);

	UPROPERTY()
	FMoleCollected OnMoleCollected;

	UFUNCTION()
	void SpawnMole() const;

	UFUNCTION()
	void DespawnMole() const;

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
};
