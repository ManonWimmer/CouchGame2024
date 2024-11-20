// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoleSpawner.generated.h"

class AMoleElement;
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
	void StartCountdownStaying();

	UFUNCTION()
	void SpawnMole();
	
	UFUNCTION()
	void DestroyMole();

	UFUNCTION()
	void GetRandomSpawnTime();

	UFUNCTION()
	void GetRandomStayTime();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMoleElement> MoleClass;

private:
	UPROPERTY(EditAnywhere)
	float MinMoleSpawnTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxMoleSpawnTime = 20.0f;
	
	UPROPERTY()
	float RandomSpawnTime = 0.f;

	UPROPERTY(EditAnywhere)
	float MinMoleStayTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxMoleStayTime = 20.0f;

	UPROPERTY()
	float RandomStayTime = 0.f;

	UPROPERTY()
	float CurrentTime = 0.f;

	UPROPERTY()
	float StartedTime = 0.f;

	UPROPERTY()
	bool bCanSpawn = false;

	UPROPERTY()
	bool bCanStay = false;

	UPROPERTY()
	AMoleElement* SpawnedMole = nullptr;
};
