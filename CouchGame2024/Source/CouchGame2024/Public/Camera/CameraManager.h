// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraManager.generated.h"

UCLASS()
class COUCHGAME2024_API ACameraManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> CameraClass;

private:
	void GetAllPlayers();

	void GetMainCamera();

	UPROPERTY()
	TArray<AActor*> Players;

	UPROPERTY()
	AActor* MainCamera;
};
