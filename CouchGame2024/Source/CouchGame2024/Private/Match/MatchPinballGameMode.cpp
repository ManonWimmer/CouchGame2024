// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchPinballGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Match/MatchSettings.h"
#include "Match/PlayerBallSpawn.h"
#include "PlayerBall/PlayerBall.h"

void AMatchPinballGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateAndInitPlayers();

	TArray<APlayerBallSpawn*> PlayerSpawnPoints;
	FindPlayerBallSpawnInWorld(PlayerSpawnPoints);
	SpawnPlayerBalls(PlayerSpawnPoints);
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
	
	float Size = ResultsActors.Num();
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Green,
		FString::Printf(TEXT("Number of player balls spawn found : %f"), Size)
	);
}

void AMatchPinballGameMode::SpawnPlayerBalls(const TArray<APlayerBallSpawn*> SpawnPoints)
{
	for (APlayerBallSpawn* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();

		TSubclassOf<APlayerBall> PlayerBallClass = GetPlayerBallClassFromInputType(InputType);

		if (PlayerBallClass == nullptr) continue;

		APlayerBall* NewCharacter = GetWorld()->SpawnActorDeferred<APlayerBall>(PlayerBallClass, SpawnPoint->GetTransform());

		if (NewCharacter == nullptr) continue;
		
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		PlayersBallInsideArena.Add(NewCharacter);
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
