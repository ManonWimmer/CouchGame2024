// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireworkElement.generated.h"

UCLASS()
class COUCHGAME2024_API AFireworkElement : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireworkElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void FireFirework();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFireFirework"))
	void ReceiveOnFireFirework();


	UFUNCTION()
	void StopFirework();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStopFirework"))
	void ReceiveOnStopFirework();
};
