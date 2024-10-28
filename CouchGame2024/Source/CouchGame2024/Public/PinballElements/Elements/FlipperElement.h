// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "FlipperElement.generated.h"

class UBoxComponent;

UCLASS()
class COUCHGAME2024_API AFlipperElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlipperElement();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> FlipperMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> FlipperMeshRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> FlipperBoxTrigger;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TriggerElement() override;
	
	virtual EPinballElementID GetElementID() override;

private:
	UFUNCTION(BlueprintCallable)
	void ActivateFlip();

	UFUNCTION()
	void HandleFlipRotation(float DeltaTime);
	
	UPROPERTY()
	bool IsFlipping;
	
	float CurrentRotationAngle = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Flipper Settings")
	float FlipperMaxRotationAngle = 45.0f;
	UPROPERTY(EditAnywhere, Category = "Flipper Settings")
	float RotationSpeed = 90.0f;
};
