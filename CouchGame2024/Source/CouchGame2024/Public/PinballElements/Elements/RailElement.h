// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "RailElement.generated.h"

class USplineComponent;
class USphereComponent;

UCLASS()
class COUCHGAME2024_API ARailElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARailElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	virtual EPinballElementID GetElementID() override;

	virtual void TriggerElement() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereTriggerRail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereTriggerRailInverse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USplineComponent> SplineRail;

	UPROPERTY()
	TObjectPtr<USceneComponent> RootSceneComp;

public:

	UFUNCTION()
	FVector GetLocationAlongRailSpline(float percent);

	UFUNCTION()
	float GetRailProgressDuration();

	UFUNCTION()
	FVector GetTangentAtSplinePercent(float Percent);

private:

	UPROPERTY(EditAnywhere)
	float RailProgressDuration = 1.f;
};

inline float ARailElement::GetRailProgressDuration()
{
	return RailProgressDuration;
}
