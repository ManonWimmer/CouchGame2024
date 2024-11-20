// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoleSpawner.generated.h"

class APowerUpMole;

UCLASS()
class COUCHGAME2024_API AMoleSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMoleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StartCountdownSpawning();

	UFUNCTION()
	void StopCountdownSpawning();

	UFUNCTION()
	void SpawnMole();

	UFUNCTION()
	void GetRandomSpawnTime();

	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpMole> MoleClass;

private:
	UPROPERTY(EditAnywhere)
	float MinMoleSpawnTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxMoleSpawnTime = 20.0f;
	
	UPROPERTY()
	float RandomSpawnTime = 0.f;

	UPROPERTY()
	float CurrentTime = 0.f;

	UPROPERTY()
	float StartedTime = 0.f;

	UPROPERTY()
	bool bCanSpawn = false;

	UPROPERTY()
	APowerUpMole* SpawnedMole = nullptr;
};
