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


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMoveXInput, float, XInput);

	UPROPERTY(BlueprintCallable)
	FOnPlayerMoveXInput OnPlayerMoveXInput;
	
protected:
	UFUNCTION(BlueprintCallable)
	void MoveXInput(float XInput);

	
};
