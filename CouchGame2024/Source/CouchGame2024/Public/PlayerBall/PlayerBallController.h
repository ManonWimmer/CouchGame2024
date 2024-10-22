// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBallController.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API APlayerBallController : public APlayerController
{
	GENERATED_BODY()

#pragma region Movements Input
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoveXInput, float, XInput);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoveYInput, float, YInput);

	UPROPERTY(BlueprintCallable)
	FOnPlayerMoveXInput OnPlayerMoveXInput;
	UPROPERTY(BlueprintCallable)
	FOnPlayerMoveYInput OnPlayerMoveYInput;
	
protected:
	UFUNCTION(BlueprintCallable)
	void MoveXInput(float XInput);
	UFUNCTION(BlueprintCallable)
	void MoveYInput(float YInput);

	
#pragma endregion
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPunchInput, float, PunchInput);

	UPROPERTY(BlueprintCallable)
	FOnPlayerPunchInput OnPlayerPunchInput;
};
