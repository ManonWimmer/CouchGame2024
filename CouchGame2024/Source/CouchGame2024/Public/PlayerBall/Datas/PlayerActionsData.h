// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "PlayerActionsData.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerActionsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MoveX")
	TObjectPtr<UInputAction> MoveXAction;

	UPROPERTY(EditAnywhere, Category = "MoveY")
	TObjectPtr<UInputAction> MoveYAction;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	TObjectPtr<UInputAction> GrapplingAction;
	
	/*
	UPROPERTY(EditAnywhere, Category = "Punch")
	TObjectPtr<UInputAction> PunchAction;
	*/
};
