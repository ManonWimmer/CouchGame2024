// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hookable.h"
#include "GameFramework/Actor.h"
#include "TestHookPoint.generated.h"

UCLASS()
class COUCHGAME2024_API ATestHookPoint : public AActor, public IHookable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestHookPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interface
	virtual FVector GetHookPosition() override;

	virtual bool IsHookable() override;
};
