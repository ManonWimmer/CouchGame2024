// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

// Création des PlayerControllers en fonction du nombre de settings clavier / manettes disponibles dans les paramètres.
void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	
	for (int i = 0; i < MultiplayerSettings->NbMaxGamepads; i++)
	{
		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);
	}
	for (int i = MultiplayerSettings->NbMaxGamepads; i < MultiplayerSettings->NbMaxGamepads + MultiplayerSettings->GetNbKeyboardProfiles(); i++)
	{
		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);
	}

	// Faire pour deuxieme partie
}

// Retrouve l’index d’un PlayerController à partir de l’index d’un profil clavier (retourne -1 s’il n’existe pas encore de mapping).
int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if (!PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))	return -1;
	
	return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
}

// Assigne un nouvel index d’un PlayerController pour l’index du profil clavier fourni (retourne le joueur qui vient d’être assigné).
int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

// Associe l’IMC correspondant à l’index du profil clavier fourni à l’index du Player Controller fourni.
void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	UInputMappingContext* IMC = MultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	
	APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

	if (Controller == nullptr)	return;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, Controller->GetPawn()->GetName());
	
	FModifyContextOptions ContextOptions;

	ContextOptions.bForceImmediately = true;

	if (Controller->GetLocalPlayer())
	{
		Controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, PlayerIndex + 1, ContextOptions);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadProfileID(int DeviceID)
{
	if (!PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))	return -1;
	
	return PlayerIndexFromGamepadProfileIndex[DeviceID];
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadProfileID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	UInputMappingContext* IMC = MultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType);
	
	APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

	FModifyContextOptions ContextOptions;

	ContextOptions.bForceImmediately = true;

	if (Controller->GetLocalPlayer())
	{
		Controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, PlayerIndex + 1, ContextOptions);
	}
}

