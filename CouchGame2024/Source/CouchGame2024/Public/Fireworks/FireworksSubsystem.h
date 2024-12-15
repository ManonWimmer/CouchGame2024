// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FireworksSubsystem.generated.h"

class AFireworkElement;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UFireworksSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	UFUNCTION()
	void InitFireworkSubsystem();


	UFUNCTION(BlueprintCallable)
	void FireAllFireworks();

	UFUNCTION(BlueprintCallable)
	void StopAllFireworks();
	
private:
	UPROPERTY()
	TArray<AFireworkElement*> FireworkElements;
};
