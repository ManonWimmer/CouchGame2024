// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "Rounds/RoundsResetable.h"
#include "PlayerBallStateTourniquet.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallStateTourniquet : public UPlayerBallState
{
	GENERATED_BODY()


public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	
private:
	
	UFUNCTION()
	void EnterTourniquet();
	
	UFUNCTION()
	void ExitTourniquet(float ExitValues);
	
};
