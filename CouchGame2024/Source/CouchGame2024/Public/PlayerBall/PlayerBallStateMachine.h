// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBallStateID.h"
#include "UObject/Object.h"
#include "PlayerBallStateMachine.generated.h"

class APlayerBall;
class UPlayerBallState;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallStateMachine : public UObject
{
	GENERATED_BODY()

public:

	void Init(APlayerBall* InPawn);

	APlayerBall* GetPawn() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(EPlayerBallStateID NextStateID);

	UPlayerBallState* GetState(EPlayerBallStateID StateID);

	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	UPlayerBallState* GetCurrentState() const;

	UFUNCTION()
	EPlayerBallStateID GetCurrentStateID() const;
	
protected:

	UPROPERTY()
	TObjectPtr<APlayerBall> Pawn;

	UPROPERTY()
	TArray<UPlayerBallState*> AllStates;

	UPROPERTY()
	EPlayerBallStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<UPlayerBallState> CurrentState;

	void FindStates();

	void InitStates();
};
