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
	ACameraActor* MainCamera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DebugSphereClass;

	UPROPERTY(EditAnywhere)
	float CameraPitchMultiplier = 0.3f;
	
	UPROPERTY(EditAnywhere)
	float CameraDistance = 1000.f;

private:
	void GetAllPlayers();

	void CalculateCameraLocations();
	void MoveCamera(float DeltaTime);

	void GetDebugSphere();
	void MoveDebugSphere();
	
	UPROPERTY()
	TArray<AActor*> Players;
	
	UPROPERTY()
	FVector AveragePlayerPositions;

	//UPROPERTY()
	//FVector NewCameraLocation;

	UPROPERTY()
	AActor* DebugSphere;

	float MoveSpeed = 5.f;
};
