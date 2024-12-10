// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpID.h"
#include "Events/EventData.h"
#include "GameFramework/Actor.h"
#include "SpawnerPowerUp.generated.h"

class USpawnerPowerUpData;
class UPowerUpDataType;
class APowerUp;

UCLASS()
class COUCHGAME2024_API ASpawnerPowerUp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnerPowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BindToEventsManager();


private:

	void SetupData();
	
	UFUNCTION()
	void ReceiveSpawnerPowerUpCollected();
	
	UFUNCTION()
	void BindStartSpawnToPickPowerUp(APowerUp* InPowerUp);
	
	UFUNCTION()
	void UnbindStartSpawnToPickPowerUp(APowerUp* InPowerUp);

	UFUNCTION()
	void SpawnSpecificPowerUp(EPowerUpID InPowerUpID);

	UFUNCTION()
	void SpawnRandomPowerUp();

	UFUNCTION()
	void StartSpawning(UEventData* EventData);

	UFUNCTION()
	void StopSpawning();
	
	EPowerUpID ChooseRandomPowerUp();

	UFUNCTION()
	void HandleRespawnCooldown(float DeltaTime);

	UFUNCTION()
	void StartRespawnCooldown();

	UFUNCTION()
	void StopRespawnCooldown();

	bool bIsInCooldown = false;

	UFUNCTION()
	bool CheckCanSpawnPowerUp();

	UFUNCTION()
	void SetNewRespawnCooldownToAdd();
	
	UPROPERTY()
	TObjectPtr<APowerUp> SpawnedPowerUp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPowerUpDataType> PowerUpDataType;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpawnerPowerUpData> SpawnerPowerUpData;

	UPROPERTY()
	float CurrentRespawnCooldown = 0.f;

	UPROPERTY()
	float RespawnCooldownToAdd = 5.f;

	UPROPERTY()
	float MinRespawnCooldownToAdd = 2.f;
	
	UPROPERTY()
	float MaxRespawnCooldownToAdd = 5.f;

	UPROPERTY()
	float BasicRespawnCooldown = 5.f;
	
	UPROPERTY()
	bool bCanSpawn = false;
};
