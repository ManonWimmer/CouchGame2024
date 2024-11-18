// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElementID.h"
#include "GameFramework/Actor.h"
#include "PinballElement.generated.h"

class APlayerBall;

UCLASS()
class COUCHGAME2024_API APinballElement : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APinballElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual EPinballElementID GetElementID();

	UFUNCTION()	
	virtual void TriggerElement();

	UFUNCTION()
	virtual void TriggerElementWithPlayer(APlayerBall* InPlayerBall = nullptr);

	UFUNCTION(BlueprintImplementableEvent)
	void TriggerElementEffects();
};
