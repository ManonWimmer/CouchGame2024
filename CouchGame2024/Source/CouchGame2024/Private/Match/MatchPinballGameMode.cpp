// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchPinballGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Match/MatchSettings.h"
#include "Match/PlayerBallSpawn.h"
#include "PlayerBall/PlayerBall.h"
#include "Rounds/RoundsSubsystem.h"

void AMatchPinballGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Players
	CreateAndInitPlayers();
	TArray<APlayerBallSpawn*> PlayerSpawnPoints;
	FindPlayerBallSpawnInWorld(PlayerSpawnPoints);
	SpawnPlayerBalls(PlayerSpawnPoints);


	// Rounds
	InitRoundsSubsystem();
}

void AMatchPinballGameMode::FindPlayerBallSpawnInWorld(TArray<APlayerBallSpawn*>& ResultsActors)
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

void AMatchPinballGameMode::SpawnPlayerBalls(const TArray<APlayerBallSpawn*> SpawnPoints)
{
	int PlayerIndex = 0;
	for (APlayerBallSpawn* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();

		TSubclassOf<APlayerBall> PlayerBallClass = GetPlayerBallClassFromInputType(InputType);

		if (PlayerBallClass == nullptr) continue;

		APlayerBall* NewCharacter = GetWorld()->SpawnActorDeferred<APlayerBall>(PlayerBallClass, SpawnPoint->GetTransform());

		NewCharacter->PlayerIndex = PlayerIndex;
		
		if (NewCharacter == nullptr) continue;
		
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		PlayersBallInsideArena.Add(NewCharacter);
		PlayerIndex++;
	}
}

TSubclassOf<APlayerBall> AMatchPinballGameMode::GetPlayerBallClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	const UMatchSettings* MatchSettings = GetDefault<UMatchSettings>();

	if (MatchSettings == nullptr)	return nullptr;

	switch (InputType)
	{
		case EAutoReceiveInput::Type::Player0:
			return MatchSettings->PlayerBallClassP0;
		case EAutoReceiveInput::Type::Player1:
			return MatchSettings->PlayerBallClassP1;
		case EAutoReceiveInput::Type::Player2:
			return MatchSettings->PlayerBallClassP2;
		case EAutoReceiveInput::Type::Player3:
			return MatchSettings->PlayerBallClassP3;

		default:
			return nullptr;
	}
}

void AMatchPinballGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

void AMatchPinballGameMode::InitRoundsSubsystem()
{
	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Get Round subsystem");
	
	RoundsSubsystem->InitRoundSubsystem();
}
