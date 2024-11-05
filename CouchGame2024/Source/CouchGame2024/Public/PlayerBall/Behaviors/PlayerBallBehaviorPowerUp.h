// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallBehaviorComponent.h"
#include "PlayerBallBehaviorPowerUp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallBehaviorPowerUp : public UPlayerBallBehaviorComponent
{
	GENERATED_BODY()

public:
	UPlayerBallBehaviorPowerUp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void InitBehavior() override;

	virtual void BindBehaviorEventAction(APlayerBallController* InPlayerBallController) override;

	
};
