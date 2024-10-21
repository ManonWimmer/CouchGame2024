﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateIdle.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallStateIdle : public UPlayerBallState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBallStateIdle();

public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnMoveXCheck();
};
