// Fill out your copyright notice in the Description page of Project Settings.


#include "Rounds/RoundsSubsystem.h"

#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Match/MatchPinballGameMode.h"
#include "PlayerBall/LockableInput.h"
#include "Rounds/RoundsResetable.h"
#include "Rounds/RoundsSettings.h"
#include "Rounds/Datas/RoundsData.h"
#include "Score/GlobalScoreSubsystem.h"
#include "Sounds/SoundSubsystem.h"


void URoundsSubsystem::Tick(float DeltaTime)
{
	switch (CurrentRoundPhaseID)
	{
		case NONE:
			break;
		case PRE_ROUND:
			HandlePreRoundTimer(DeltaTime);
			break;
		case STARTING_ROUND:
			HandleStartingRoundTimer(DeltaTime);
			break;
		case IN_ROUND:
			break;
		case POST_ROUND:
			//HandlePostRoundTimer(DeltaTime);
			break;
	}
}

void URoundsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
}

void URoundsSubsystem::InitRoundSubsystem()
{
	InitFirstPlayerSpecial();	// Set a lastPlayerWin (playerSpecial) for 1st round

	InitRounds();	// Init index rounds
	
	InitRoundsWonByPlayers(4);	// Init all data for RoundsWon
	
	InitTimers();	// (Re)set timers by data

	InitRoundsPhase();	// Init rounds phases data

	InitSoundRoundEffects();	// Get sounds subsystem to call sounds from here
	
	InitGamemodeAndBindStartGame();	// Set Gamemode and bind start 1st round to beginGame

	InitGlobalScoreAndReset();	// Set GlobalScore and reset it
}

void URoundsSubsystem::StartRound()	// Reset AllScores -> Lock players -> set to startLocation	-> roundphase Pre_Round
{
	if (GlobalScoreSubsystem != nullptr)
	{
		GlobalScoreSubsystem->ResetAllScores();
	}
	
	LockAllPlayer();
	
	if (MatchPinballGameMode != nullptr)
	{
		MatchPinballGameMode->SetNewLocationStartPlayerBallsSpecial(PlayerIndexLastRoundWin);
	}

	if (bIsFirstCheckRound)
	{
		ChangeRoundPhase(ERoundsPhaseID::POST_ROUND);
	}
	else
	{
		ChangeRoundPhase(ERoundsPhaseID::PRE_ROUND);

		if (SoundSubsystem != nullptr)
		{
			SoundSubsystem->PlayRoundStartSound();
		}
	}
}

void URoundsSubsystem::EndRoundChecks()	// Check best player & increments rounds Win
{
	if (!bIsFirstCheckRound)
	{
		PlayerIndexLastRoundWin = GlobalScoreSubsystem->GetIndexPlayerBestScore();

		IncreaseRoundsWonByPlayerIndex(PlayerIndexLastRoundWin, 1);
	}
	else
	{
		bIsFirstCheckRound = false;
	}

	if (SoundSubsystem != nullptr)
	{
		SoundSubsystem->PlayCrowdEffectSound();
	}
}

void URoundsSubsystem::InitTimers()	// (Re)set timers by data
{
	const URoundsSettings* RoundsSettings = GetDefault<URoundsSettings>();

	if (RoundsSettings == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Warning : Rounds Settings is nullptr");
		return;
	}

	URoundsData* RoundsData = RoundsSettings->RoundsData.LoadSynchronous();
	
	if (RoundsData == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Warning : Rounds Settings Data is nullptr");
		return;
	}

		
	CurrentPreRoundTimer = RoundsData->PreRoundDuration;
	CurrentStartingRoundTimer = RoundsData->StartingRoundDuration;
	CurrentPostRoundTimer = RoundsData->PostRoundDuration;

	TotalRoundsToWin = RoundsData->RoundsToWin;
}

void URoundsSubsystem::InitRounds()	// Init rounds index
{
	CurrentRoundIndex = 0;
}

void URoundsSubsystem::InitFirstPlayerSpecial()	// Init a first player randomly for first round
{
	int RandomIndex = 0;

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance != nullptr)
	{
		ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
		if (LocalMultiplayerSubsystem != nullptr)
		{
			RandomIndex = LocalMultiplayerSubsystem->GetRandomPlayersIndexConnected();
		}
	}

	RandomIndex = FMath::Clamp(RandomIndex, 0, 3);
	
	if (PlayerIndexLastRoundWin < 0)
	{
		PlayerIndexLastRoundWin = UKismetMathLibrary::RandomIntegerInRange(0, RandomIndex);
	}
}

void URoundsSubsystem::InitGamemodeAndBindStartGame()	// Set Gamemode and bind start 1st round to beginGame
{
	if (GetWorld() == nullptr)	return;
	
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());

	if (GameMode != nullptr)
	{
		MatchPinballGameMode = Cast<AMatchPinballGameMode>(GameMode);

		if (MatchPinballGameMode != nullptr)
		{
			MatchPinballGameMode->OnBeginGameMatch.AddDynamic(this, &URoundsSubsystem::StartRound);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Warning : Match Pinball GameMode not found in roundsSubsystem");
		}
	}
}

void URoundsSubsystem::InitGlobalScoreAndReset()	// Set GlobalScore and reset it
{
	if (GetWorld() == nullptr)	return;
	if (GetWorld()->GetGameInstance() == nullptr)	return;
	UGlobalScoreSubsystem* ScoreSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	
	if (ScoreSubsystem != nullptr)
	{
		GlobalScoreSubsystem = ScoreSubsystem;

		GlobalScoreSubsystem->ResetAllScores();
	}
}

void URoundsSubsystem::InitSoundRoundEffects()
{
	if (GetWorld() == nullptr)	return;
	if (GetWorld()->GetGameInstance() == nullptr)	return;
	SoundSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USoundSubsystem>();
}

void URoundsSubsystem::InitRoundsPhase()	// Init rounds phases data
{
	CurrentRoundPhaseID = NONE;
}

void URoundsSubsystem::ChangeRound(int NewRoundIndex)
{
	CurrentRoundIndex = NewRoundIndex;
	
	ResetRound();
	
	StartRound();
	OnChangeRound.Broadcast(CurrentRoundIndex);

	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Change Round");
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentRoundIndex));
	}
}

void URoundsSubsystem::ChangeToNextRound()
{
	ChangeRound(CurrentRoundIndex + 1);
}

int URoundsSubsystem::GetCurrentRoundIndex()
{
	return CurrentRoundIndex;
}

ERoundsPhaseID URoundsSubsystem::GetCurrentRoundPhaseID()
{
	return CurrentRoundPhaseID;
}

void URoundsSubsystem::ChangeRoundPhase(ERoundsPhaseID RoundsPhaseID)
{
	CurrentRoundPhaseID = RoundsPhaseID;
	//OnChangeRoundPhases.Broadcast(CurrentRoundPhaseID);

	switch (CurrentRoundPhaseID)
	{
		case STARTING_ROUND:
			LockAllPlayerButOne(PlayerIndexLastRoundWin);
			break;
		case IN_ROUND:
			UnlockAllPlayer();
			break;
		case POST_ROUND:
			EndRoundChecks();
			if (SoundSubsystem != nullptr)
			{
				SoundSubsystem->PlayWaitingMusicSound();
			}
		
			LockAllPlayer();
			break;
		default:
			break;
	}

	OnChangeRoundPhases.Broadcast(CurrentRoundPhaseID);
	
	if (GEngine)
	{
		const FString PhaseIdString = UEnum::GetValueAsString(CurrentRoundPhaseID);
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Change Round Phase");
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New phase Id = %s"), *PhaseIdString));
	}
}

void URoundsSubsystem::StartRoundPhase()
{
	ChangeRoundPhase(PRE_ROUND);
}

void URoundsSubsystem::ChangeToNextRoundPhase()
{
	switch (CurrentRoundPhaseID)
	{
		case NONE:
			break;
		case PRE_ROUND:
			ChangeRoundPhase(STARTING_ROUND);
			break;
		case STARTING_ROUND:
			ChangeRoundPhase(IN_ROUND);
			break;
		case IN_ROUND:
			ChangeRoundPhase(POST_ROUND);
			break;
		case POST_ROUND:
			if (!bPlayerHasWonGame)
			{
				ChangeToNextRound();
			}
			break;

		default:
			break;
	}
}

void URoundsSubsystem::InitRoundsWonByPlayers(int PlayerCount)	// Init all data for RoundsWon
{
	RoundsWonByPlayersIndex.Empty();
	
	for (int i = 0; i < PlayerCount; i++)
	{
		RoundsWonByPlayersIndex.Add(i, 0);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("New num winner : %d"), RoundsWonByPlayersIndex.Num()));

}

int URoundsSubsystem::GetRoundsWonByPlayerIndex(int PlayerIndex)
{
	if (RoundsWonByPlayersIndex.Num() <= PlayerIndex || PlayerIndex < 0)	return	-1;
	
	return RoundsWonByPlayersIndex[PlayerIndex];
}

void URoundsSubsystem::IncreaseRoundsWonByPlayerIndex(int PlayerIndex, int IncreaseNumber)
{
	if (PlayerIndex < 0)	return;
	
	if (PlayerIndex >= RoundsWonByPlayersIndex.Num())	return;

	RoundsWonByPlayersIndex[PlayerIndex] += IncreaseNumber;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("New score last winner : %d"), RoundsWonByPlayersIndex[PlayerIndex]));

		
	CheckForWinPlayer(PlayerIndex);
}

void URoundsSubsystem::CheckForWinPlayer(int PlayerIndex)
{
	if (RoundsWonByPlayersIndex.Num() <= PlayerIndex)	return;

	if (PlayerIndex < 0)	return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("New score last winner by function : %d"), GetRoundsWonByPlayerIndex(PlayerIndex)));

	
	if (GetRoundsWonByPlayerIndex(PlayerIndex) >= TotalRoundsToWin)
	{
		if (MatchPinballGameMode != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Cuurent score winner : %d"), GetRoundsWonByPlayerIndex(PlayerIndex)));
			MatchPinballGameMode->MatchWin(PlayerIndex);
			bPlayerHasWonGame = true;
		}
	}
}

void URoundsSubsystem::HandlePreRoundTimer(float DeltaTime)
{
	if (CurrentPreRoundTimer > 0.f)
	{
		CurrentPreRoundTimer -= DeltaTime;
	}
	else
	{
		ChangeToNextRoundPhase();
	}
}

void URoundsSubsystem::HandleStartingRoundTimer(float DeltaTime)
{
	if (CurrentStartingRoundTimer > 0.f)
	{
		CurrentStartingRoundTimer -= DeltaTime;
	}
	else
	{
		ChangeToNextRoundPhase();
	}
}

void URoundsSubsystem::HandlePostRoundTimer(float DeltaTime)
{
	if (CurrentPostRoundTimer > 0.f)
	{
		CurrentPostRoundTimer -= DeltaTime;
	}
	else
    {
    	ChangeToNextRoundPhase();
    }
 }

void URoundsSubsystem::AddResetableObject(UObject* InResetableObject)
{
	if (InResetableObject == nullptr)	return;
	if (ResetableObjects.Contains(InResetableObject))	return;

	ResetableObjects.Add(InResetableObject);
}

void URoundsSubsystem::RemoveResetableObjects(UObject* InResetableObject)
{
	if (InResetableObject == nullptr)	return;
	if (!ResetableObjects.Contains(InResetableObject))	return;

	ResetableObjects.Remove(InResetableObject);
}

void URoundsSubsystem::ResetRound()
{
	InitTimers();

	InitRoundsPhase();
	
	for (UObject* Resetable : ResetableObjects)
	{
		if (Resetable == nullptr)	continue;

		IRoundsResetable* RoundsResetable = Cast<IRoundsResetable>(Resetable);

		if (RoundsResetable == nullptr)	continue;

		if (!RoundsResetable->IsResetable())	continue;

		RoundsResetable->ResetObject();
	}
}

void URoundsSubsystem::AddLockableInput(UObject* Input)
{
	if (Input == nullptr)	return;

	if (LockableInputsInWorld.Contains(Input))	return;
	
	LockableInputsInWorld.Add(Input);
}

void URoundsSubsystem::RemoveLockableInput(UObject* Input)
{
	if (Input == nullptr)	return;

	if (!LockableInputsInWorld.Contains(Input))	return;

	LockableInputsInWorld.Remove(Input);
}

void URoundsSubsystem::LockAllPlayerButOne(int PlayerIndexUnlock)
{
	for (UObject* LockableInputObject : LockableInputsInWorld)
	{
		if (LockableInputObject == nullptr)	continue;

		ILockableInput* LockableInputActor = Cast<ILockableInput>(LockableInputObject);
		
		if (!LockableInputActor->IsLockableInput())	continue;
		
		if (LockableInputActor->GetLockableInputIndex() == PlayerIndexUnlock)
		{
			LockableInputActor->LockButOnlySpecialInput();
			continue;
		}

		LockableInputActor->LockInput();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Lock all players but : %d"), PlayerIndexUnlock));
}

void URoundsSubsystem::LockAllPlayer()
{
	LockAllPlayerButOne(-1);
}

void URoundsSubsystem::UnlockAllPlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Unlock all players");
	
	for (UObject* LockableInputObject : LockableInputsInWorld)
	{
		if (LockableInputObject == nullptr)	continue;

		ILockableInput* LockableInputActor = Cast<ILockableInput>(LockableInputObject);
		
		if (!LockableInputActor->IsLockableInput())	continue;
		
		LockableInputActor->UnlockInput();
	}
}

void URoundsSubsystem::SetRespawnRailElement(ARailElement* InRespawnRailElement)
{
	if (InRespawnRailElement == nullptr)	return;

	RespawnRailElement = InRespawnRailElement;
}

ARailElement* URoundsSubsystem::GetRespawnRailElement()
{
	return RespawnRailElement;
}


