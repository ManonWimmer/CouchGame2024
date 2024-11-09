// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "PillarElement.generated.h"

UCLASS()
class COUCHGAME2024_API APillarElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APillarElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
