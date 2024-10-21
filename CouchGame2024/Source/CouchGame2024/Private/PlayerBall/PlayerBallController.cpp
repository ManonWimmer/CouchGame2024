// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"

void APlayerBallController::MoveXInput(float XInput)
{
	OnPlayerMoveX.Broadcast(XInput);
}
