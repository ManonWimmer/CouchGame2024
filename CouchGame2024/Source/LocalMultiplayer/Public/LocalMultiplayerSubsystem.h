// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"

enum class ELocalMultiplayerInputMappingType;
/**
 * 
 */
UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType, bool AutoReasign = true);

	void ReasignExistingPlayers(ELocalMultiplayerInputMappingType MappingType);
	
	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);

	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);

	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;

	int GetAssignedPlayerIndexFromGamepadProfileID(int DeviceID);

	int AssignNewPlayerToGamepadProfileID(int DeviceID);

	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;


	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddNewPlayerForController, int, PlayerIndex);
	FOnAddNewPlayerForController OnAddNewPlayerForController;


	bool AllowNewPlayerConnection = true;
	
protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = -1;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};
