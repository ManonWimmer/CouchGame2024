// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"

void APlayerBallController::MoveXInput(float XInput)
{
	OnPlayerMoveXInput.Broadcast(XInput);
}

void APlayerBallController::MoveYInput(float YInput)
{
	OnPlayerMoveYInput.Broadcast(YInput);
}
