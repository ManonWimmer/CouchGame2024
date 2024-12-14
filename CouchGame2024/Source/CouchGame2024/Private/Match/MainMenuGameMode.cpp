// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MainMenuGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Match/MatchSettings.h"
#include "Match/PlayerBallSpawn.h"
#include "PinballElements/Elements/RailElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "Skins/SkinsSubsystem.h"
#include "Sounds/SoundSubsystem.h"


void AMainMenuGameMode::InitSoundSubsystem()
{
	if (GetGameInstance() == nullptr)	return;
	
	USoundSubsystem* SoundSubsystem = GetGameInstance()->GetSubsystem<USoundSubsystem>();

	if (SoundSubsystem == nullptr)	return;

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Sound Subsystem !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	SoundSubsystem->InitSoundSubsystem();
	SoundSubsystem->InitMusicAudioComponent();
}

void AMainMenuGameMode::InitSkinsSubsystem()
{
	if (GetGameInstance() == nullptr)	return;
	
	USkinsSubsystem* SkinsSubsystem = GetGameInstance()->GetSubsystem<USkinsSubsystem>();

	if (SkinsSubsystem == nullptr)	return;

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Skins Subsystem !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	SkinsSubsystem->InitSkinsSubsystem();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitSoundSubsystem();
	InitSkinsSubsystem();
	
	// Connection
	InitConnectionRailElements();
	BindSpawnPlayerBallToNewPlayerConnected();
	
	// Players
	CreateAndInitPlayers();
	TArray<APlayerBallSpawn*> PlayerSpawnPoints;
	FindPlayerBallSpawnInWorld(PlayerSpawnPoints);
	PlayerBallSpawns = PlayerSpawnPoints;
	SpawnPlayerBalls(PlayerSpawnPoints);
	
	OnPlayerBallSpawned.Broadcast();

	DisconnectAllPlayersBall();
	
	ReasignExistingPlayers();
}

void AMainMenuGameMode::FindPlayerBallSpawnInWorld(TArray<APlayerBallSpawn*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBallSpawn::StaticClass(), FoundActors);

	
	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		APlayerBallSpawn* PlayerSpawn = Cast<APlayerBallSpawn>(FoundActors[i]);

		if (PlayerSpawn == nullptr)
			continue;

		ResultsActors.Add(PlayerSpawn);
	}
}

void AMainMenuGameMode::SpawnPlayerBalls(const TArray<APlayerBallSpawn*> SpawnPoints)
{
	//int PlayerIndex = 0;
	for (APlayerBallSpawn* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		
		TSubclassOf<APlayerBall> PlayerBallClass = GetPlayerBallClassFromInputType(InputType);

		if (PlayerBallClass == nullptr) continue;

		APlayerBall* NewCharacter = GetWorld()->SpawnActorDeferred<APlayerBall>(PlayerBallClass, SpawnPoint->GetTransform());

		int PlayerIndex = GetPlayerIndexFromInputType(InputType);
		NewCharacter->PlayerIndex = PlayerIndex;
		
		if (NewCharacter == nullptr) continue;
		
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		PlayersBallInsideArena.Add(NewCharacter);
		//PlayerIndex++;
	}

	Algo::Sort(PlayersBallInsideArena, [](const APlayerBall* A, const APlayerBall* B)
	{
		if (!A || !B)	return false;

		 return A->PlayerIndex < B->PlayerIndex;
	});
}

void AMainMenuGameMode::SetLocationStartPlayerBallsSpecial(const TArray<APlayerBallSpawn*> SpawnPoints, int PlayerSpecial)
{
	int PlayerCount = 0;
	
	bool bHasUsedSpecial = false;

	for (APlayerBall* PlayerBall : PlayersBallInsideArena)
	{
		if (PlayerBall == nullptr)	continue;

		PlayerBall->SetActorHiddenInGame(true);
	}
	
	for (APlayerBallSpawn* SpawnPoint : SpawnPoints)
	{
		if (PlayerCount == PlayerSpecial)
		{
			PlayerCount++;
		}
		
		if (SpawnPoint == nullptr)	continue;

		if (SpawnPoint->ActorHasTag(TEXT("SpecialSpawn")) && !bHasUsedSpecial)
		{
			if (PlayersBallInsideArena.Num() > PlayerSpecial)
			{
				if (PlayersBallInsideArena[PlayerSpecial] != nullptr)
				{
					bHasUsedSpecial = true;
					PlayersBallInsideArena[PlayerSpecial]->SetActorLocation(SpawnPoint->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);

					if (PlayersBallInsideArena[PlayerSpecial]->BehaviorMovements != nullptr)
					{
						PlayersBallInsideArena[PlayerSpecial]->BehaviorMovements->SpecialSpawnForceDir = SpawnPoint->GetSpawnSpecialForceDiretion();
					}
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Warning : Player special spawn too high");
			}
		}
		else
		{
			if (PlayersBallInsideArena.Num() > PlayerCount)
			{
				if (PlayersBallInsideArena[PlayerCount] != nullptr)
				{
					PlayersBallInsideArena[PlayerCount]->SetActorLocation(SpawnPoint->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);

					PlayerCount++;
				}
			}
		}
	}
	if (!bHasUsedSpecial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange, "Warning : No SpecialSpawn set, spawn might have bugging behaviors !");

		if (PlayersBallInsideArena.Num() > PlayerSpecial && SpawnPoints.Num() > 0)
		{
			if (PlayersBallInsideArena[PlayerSpecial] != nullptr)
			{
				PlayersBallInsideArena[PlayerSpecial]->SetActorLocation(SpawnPoints[SpawnPoints.Num()-1]->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Spawn actor : %d"), PlayerSpecial));
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("To spawn : %s"), *SpawnPoints[SpawnPoints.Num()-1]->GetName()));
			}
		}
	}

	for (APlayerBall* PlayerBall : PlayersBallInsideArena)
	{
		if (PlayerBall == nullptr)	continue;

		PlayerBall->SetActorHiddenInGame(false);
	}
}

TSubclassOf<APlayerBall> AMainMenuGameMode::GetPlayerBallClassFromInputType(EAutoReceiveInput::Type InputType) const
{
	const UMatchSettings* MatchSettings = GetDefault<UMatchSettings>();

	if (MatchSettings == nullptr)	return nullptr;

	switch (InputType)
	{
	case EAutoReceiveInput::Type::Player0:
		return MatchSettings->PlayerBallClassP0.LoadSynchronous();
	case EAutoReceiveInput::Type::Player1:
		return MatchSettings->PlayerBallClassP1.LoadSynchronous();
	case EAutoReceiveInput::Type::Player2:
		return MatchSettings->PlayerBallClassP2.LoadSynchronous();
	case EAutoReceiveInput::Type::Player3:
		return MatchSettings->PlayerBallClassP3.LoadSynchronous();

	default:
		return nullptr;
	}
}

int AMainMenuGameMode::GetPlayerIndexFromInputType(EAutoReceiveInput::Type InputType) const
{
	switch (InputType)
	{
	case EAutoReceiveInput::Type::Player0:
		return 0;
	case EAutoReceiveInput::Type::Player1:
		return 1;
	case EAutoReceiveInput::Type::Player2:
		return 2;
	case EAutoReceiveInput::Type::Player3:
		return 3;

	default:
		return -1;
	}
}

void AMainMenuGameMode::SetNewLocationStartPlayerBallsSpecial(int PlayerSpecial)
{
	if (PlayerSpecial < 0)	return;
	
	SetLocationStartPlayerBallsSpecial(PlayerBallSpawns, PlayerSpecial);
}

void AMainMenuGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::Menu, false);
}

void AMainMenuGameMode::ReasignExistingPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->ReasignExistingPlayers(ELocalMultiplayerInputMappingType::Menu);
}

void AMainMenuGameMode::BindSpawnPlayerBallToNewPlayerConnected()
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->AllowNewPlayerConnection = true;	// So that new player can connect to the game and have their pawn spawn
	
	LocalMultiplayerSubsystem->OnAddNewPlayerForController.AddDynamic(this, &AMainMenuGameMode::SpawnNewPlayerBallByPlayerConnected);
}

void AMainMenuGameMode::SpawnNewPlayerBallByPlayerConnected(int InPlayerIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "receive connect controller for spawn");
	
	if (InPlayerIndex < 0)	return;
	
	if (PlayersBallInsideArena.Num() == 0)	return;
	if (PlayersBallInsideArena.Num() <= InPlayerIndex)	return;
	
	APlayerBall* PlayerBall = PlayersBallInsideArena[InPlayerIndex];

	if (PlayerBall == nullptr)	return;

	PlayerBall->ConnectPlayer();

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->AddPlayerIndexToConnected(InPlayerIndex);
}

void AMainMenuGameMode::InitConnectionRailElements()
{
	ConnectionRailElements.Empty();
	
	TArray<AActor*> ConnectionRail;
	UGameplayStatics::GetAllActorsWithTag(this, "ConnectionRail", ConnectionRail);

	for (AActor* RailActor : ConnectionRail)
	{
		if (RailActor == nullptr)	continue;

		ARailElement* RailElement = Cast<ARailElement>(RailActor);

		if (RailElement == nullptr)	continue;

		ConnectionRailElements.Add(RailElement);
	}
}

void AMainMenuGameMode::DisconnectAllPlayersBall()
{
	for (APlayerBall* PlayerBall : PlayersBallInsideArena)
	{
		if (PlayerBall == nullptr)	continue;

		PlayerBall->DisconnectPlayer(0.f);
	}
}

ARailElement* AMainMenuGameMode::GetConnectionRailElements(int InIndex) const
{
	if (ConnectionRailElements.Num() <= 0) return nullptr;
	if (InIndex < 0)	return nullptr;

	if (InIndex >= ConnectionRailElements.Max())	return nullptr;

	return ConnectionRailElements[InIndex];
}
