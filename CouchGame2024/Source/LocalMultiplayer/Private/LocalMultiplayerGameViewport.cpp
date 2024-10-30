// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewport.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"


// passer le nombre de joueurs max de 4 à 8 (attribut MaxSplitscreenPlayers)
void ULocalMultiplayerGameViewport::PostInitProperties()
{
	Super::PostInitProperties();

	MaxSplitscreenPlayers = 8;
}

// Détection des inputs + attribution des players index en fonction de la touche du clavier / bouton de la manette fourni en paramètre.
bool ULocalMultiplayerGameViewport::InputKey(const FInputKeyEventArgs& EventArgs)
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::Printf(TEXT("TEST0")));
	ULocalMultiplayerSubsystem* MultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();

	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Key pressed"));
	
	if (!EventArgs.IsGamepad())
	{
		int KeyboardIndex = Settings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		
		if (KeyboardIndex >= 0)
		{
			int PlayerIndex = MultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardIndex);
			if (PlayerIndex == -1)
			{
				PlayerIndex = MultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboardIndex);
				MultiplayerSubsystem->AssignKeyboardMapping(PlayerIndex, KeyboardIndex, ELocalMultiplayerInputMappingType::InGame);
			}
			APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

			if (Controller != nullptr)
			{
				FInputKeyParams KeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed);
			
				return Controller->InputKey(KeyParams);
			}
		}
		else
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("Bindings not in mapping context"));
		}
	}
	else
	{
		int PlayerIndex = MultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadProfileID(EventArgs.InputDevice.GetId());

		if (PlayerIndex == -1)
		{

			PlayerIndex = MultiplayerSubsystem->AssignNewPlayerToGamepadProfileID(EventArgs.InputDevice.GetId());
			//GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Yellow,FString::Printf(TEXT("Player Index : %d"), PlayerIndex));
			MultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
		
		APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

		if (Controller)
		{
			FInputKeyParams KeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed);
			return Controller->InputKey(KeyParams);
		}

	}
	
	return Super::InputKey(EventArgs);
}

// Détection des inputs + attributions des player index en fonction des axes (joysticks à la manette) fourni en paramètre.
bool ULocalMultiplayerGameViewport::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta,
	float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if(bGamepad)
	{
		ULocalMultiplayerSubsystem* MultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();

		const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
		
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Axis pressed"));
		int PlayerIndex = MultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadProfileID(InputDevice.GetId());

		if (PlayerIndex == -1)
		{

			PlayerIndex = MultiplayerSubsystem->AssignNewPlayerToGamepadProfileID(InputDevice.GetId());
			MultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
		APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);

		if (Controller)
		{
			FInputKeyParams KeyParams(Key, Delta, DeltaTime, NumSamples, bGamepad, InputDevice);
			return Controller->InputKey(KeyParams);
		}
	}
	
	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	
}
