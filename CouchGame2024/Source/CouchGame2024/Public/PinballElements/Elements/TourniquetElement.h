// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "TourniquetElement.generated.h"

class USphereComponent;

UCLASS()
class COUCHGAME2024_API ATourniquetElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATourniquetElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	virtual EPinballElementID GetElementID() override;

	virtual void TriggerElement() override;

#pragma region Components
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereTrigger;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorTurning;

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer0;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer3;

	

#pragma endregion


#pragma region Turning


	
private:
	void HandleTurningEffect(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float TurningSpeed = 30.f;

#pragma endregion 
	
};
