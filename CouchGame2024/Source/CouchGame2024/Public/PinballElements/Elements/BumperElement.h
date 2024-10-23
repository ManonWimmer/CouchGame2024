// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "BumperElement.generated.h"

UCLASS()
class COUCHGAME2024_API ABumperElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABumperElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TriggerElement() override;
	
	virtual EPinballElementID GetElementID() override;
	
private:

	UFUNCTION()
	void BumpEffect();
};
