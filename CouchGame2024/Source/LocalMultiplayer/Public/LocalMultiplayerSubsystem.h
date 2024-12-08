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
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

#pragma region Connected / Disconnected
	UFUNCTION()
	void HandleControllerConnectionChange(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId);

	void HandlePlayerDisconnected(FInputDeviceId InputDeviceId);

	void HandlePlayerConnected(FInputDeviceId InputDeviceId);

	bool GetIsConnected(FInputDeviceId InputDeviceId);

	UPROPERTY()
	TMap<int32, EInputDeviceConnectionState> DevicesConnectionState;
	
#pragma endregion
	
	
	UFUNCTION(BlueprintCallable)
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType, bool AutoReasign = true);

	void ReasignExistingPlayers(ELocalMultiplayerInputMappingType MappingType);
	
	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);

	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);

	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;

	int GetAssignedPlayerIndexFromGamepadProfileID(int DeviceID);

	int AssignNewPlayerToGamepadProfileID(int DeviceID);

	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;



	UPROPERTY()
	TArray<int>	AllPlayerIndexUnavailable;
	int GetFirstPlayerIndexAvailable();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddNewPlayerForController, int, PlayerIndex);
	FOnAddNewPlayerForController OnAddNewPlayerForController;

	bool AllowNewPlayerConnection = true;

	void AddPlayerIndexToConnected(int InPlayerIndex);
	void RemovePlayerIndexFromConnected(int InPlayerIndex);
	void EmptyPLayersIndexConnected();
	int GetTotalOfPlayersConnected();
	int GetRandomPlayersIndexConnected();
	UPROPERTY()
	TArray<int> PlayersIndexConnected;
	
protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = -1;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};

