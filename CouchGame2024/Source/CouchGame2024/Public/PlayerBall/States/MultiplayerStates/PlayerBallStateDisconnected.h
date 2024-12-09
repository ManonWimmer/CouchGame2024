// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateDisconnected.generated.h"

class ARailElement;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallStateDisconnected : public UPlayerBallState
{
	GENERATED_BODY()

public:
	
	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;


private:

	void DisconnectBall();
	
	void ExitDisconnect();

	void InitRailRespawn();

	UPROPERTY()
	TObjectPtr<ARailElement> SpawnRailElement;
};
