// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpID.h"
#include "GameFramework/Actor.h"
#include "SpawnerPowerUp.generated.h"

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


private:
	UFUNCTION()
	void ReceiveSpawnerPowerUpCollected();
	
	void BindStartSpawnToPickPowerUp(APowerUp* InPowerUp);

	void UnbindStartSpawnToPickPowerUp(APowerUp* InPowerUp);
	
	void SpawnSpecificPowerUp(EPowerUpID InPowerUpID);
	
	void SpawnRandomPowerUp();

	EPowerUpID ChooseRandomPowerUp();

	void HandleRespawnCooldown(float DeltaTime);

	UFUNCTION()
	void StartRespawnCooldown();

	UFUNCTION()
	void StopRespawnCooldown();

	bool bIsInCooldown = false;
	
	bool CheckCanSpawnPowerUp();

	void SetNewRespawnCooldownToAdd();
	
	UPROPERTY()
	TObjectPtr<APowerUp> SpawnedPowerUp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPowerUpDataType> PowerUpDataType;
	

	float CurrentRespawnCooldown = 0.f;

	float RespawnCooldownToAdd = 5.f;

	float MinRespawnCooldownToAdd = 2.f;
	float MaxRespawnCooldownToAdd = 5.f;
	
	float BasicRespawnCooldown = 5.f;
};
