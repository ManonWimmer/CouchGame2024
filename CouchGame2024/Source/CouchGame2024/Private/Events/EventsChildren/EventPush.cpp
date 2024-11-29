// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventPush.h"

#include "Score/GlobalScoreSubsystem.h"


// Sets default values
AEventPush::AEventPush()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventPush::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Begin play push"));

	ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
}

// Called every frame
void AEventPush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInTickPhase1 || bInTickPhase2)
	{
		CheckAddTimeLastPushed(DeltaTime);
	}
}

void AEventPush::SetupEventPhase1()
{
	UE_LOG(LogTemp, Warning, TEXT("SetupEventPhase1 push"));
	Super::SetupEventPhase1();

	#pragma region Reset / Set TMaps
	if (LastPlayerIndexImpactingPlayerIndex.Num() > 0) LastPlayerIndexImpactingPlayerIndex.Empty();
	LastPlayerIndexImpactingPlayerIndex.Add(0,-1);
	LastPlayerIndexImpactingPlayerIndex.Add(1,-1);
	LastPlayerIndexImpactingPlayerIndex.Add(2,-1);
	LastPlayerIndexImpactingPlayerIndex.Add(3,-1);

	if (TimeSinceLastPlayerIndexImpacted.Num() > 0) TimeSinceLastPlayerIndexImpacted.Empty();
	TimeSinceLastPlayerIndexImpacted.Empty();
	TimeSinceLastPlayerIndexImpacted.Add(0,0);
	TimeSinceLastPlayerIndexImpacted.Add(1,0);
	TimeSinceLastPlayerIndexImpacted.Add(2,0);
	TimeSinceLastPlayerIndexImpacted.Add(3,0);

	if (LastPlayerIndexPunchingPlayerIndex.Num() > 0) LastPlayerIndexPunchingPlayerIndex.Empty();
	LastPlayerIndexPunchingPlayerIndex.Add(0,-1);
	LastPlayerIndexPunchingPlayerIndex.Add(1,-1);
	LastPlayerIndexPunchingPlayerIndex.Add(2,-1);
	LastPlayerIndexPunchingPlayerIndex.Add(3,-1);

	if (TimeSinceLastPlayerIndexPunched.Num() > 0) TimeSinceLastPlayerIndexPunched.Empty();
	TimeSinceLastPlayerIndexPunched.Add(0,0);
	TimeSinceLastPlayerIndexPunched.Add(1,0);
	TimeSinceLastPlayerIndexPunched.Add(2,0);
	TimeSinceLastPlayerIndexPunched.Add(3,0);
	#pragma endregion

	UE_LOG(LogTemp, Warning, TEXT("End setup push"));
}

void AEventPush::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();
	UE_LOG(LogTemp, Warning, TEXT("TriggerEventPhase1 push"));

	OnPushStartedEvent.Broadcast();
}

void AEventPush::TriggerEventPhase2()
{
	Super::TriggerEventPhase2();

	// todo: Set Material to the same one
}

void AEventPush::TickPhase1()
{
	Super::TickPhase1();
}

void AEventPush::TickPhase2()
{
	Super::TickPhase2();
}

void AEventPush::EndEvent()
{
	Super::EndEvent();

	// todo: Reset Material as before
	
	OnPushEndedEvent.Broadcast();
}

void AEventPush::CheckAddTimeLastPushed(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("CheckAddTimeLastPushed push"));

	UE_LOG(LogTemp, Warning, TEXT("SetupEventPhase1 push - LastPlayerIndexImpactingPlayerIndex: %s"), *FString::JoinBy(LastPlayerIndexImpactingPlayerIndex, TEXT(","), [](const TTuple<int, int>& Pair) { return FString::Printf(TEXT("(%d,%d)"), Pair.Key, Pair.Value); }));

	
	// Impact
	for (const TTuple<int, int> Element : LastPlayerIndexImpactingPlayerIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("Foreach LastPlayerIndexImpactingPlayerIndex"));
		const int PlayerIndexImpacted = Element.Key;
		const int PlayerIndexImpacting = Element.Value;
		UE_LOG(LogTemp, Warning, TEXT("Impacted : %i, Impacting : %i"), PlayerIndexImpacted, PlayerIndexImpacting);

		if (PlayerIndexImpacting != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerImpacting != -1"));
			// Add time
			TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacted] += DeltaTime;

			// Time Limit
			if (TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacted] > TimePushedLimit)
			{
				LastPlayerIndexImpactingPlayerIndex[PlayerIndexImpacted] = -1;
				LastPlayerIndexImpactingPlayerIndex[PlayerIndexImpacting] = -1;
				TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacted] = 0;
			}
		}
	}

	// Punch
	for (const TTuple<int, int> Element : LastPlayerIndexPunchingPlayerIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("Foreach LastPlayerIndexPunchingPlayerIndex"));
		const int PlayerIndexPunched = Element.Key;
		const int PlayerIndexPunching = Element.Value;
		UE_LOG(LogTemp, Warning, TEXT("Punched : %i, Punching : %i"), PlayerIndexPunched, PlayerIndexPunching);

		if (PlayerIndexPunching != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerPunching != -1"));
			// Add time
			TimeSinceLastPlayerIndexImpacted[PlayerIndexPunched] += DeltaTime;

			// Time Limit
			if (TimeSinceLastPlayerIndexPunched[PlayerIndexPunched] > TimePushedLimit)
			{
				LastPlayerIndexPunchingPlayerIndex[PlayerIndexPunched] = -1;
				LastPlayerIndexPunchingPlayerIndex[PlayerIndexPunching] = -1;
				TimeSinceLastPlayerIndexPunched[PlayerIndexPunched] = 0;
			}
		}
	}
}

void AEventPush::OnImpact(int PlayerIndexImpacting, int PlayerIndexImpacting2)
{
	if (!bInTickPhase1 && !bInTickPhase2) return;
	if (PlayerIndexImpacting == PlayerIndexImpacting2) return;

	UE_LOG(LogTemp, Warning, TEXT("On Impact, %i, %i"), PlayerIndexImpacting, PlayerIndexImpacting2);
	
	LastPlayerIndexImpactingPlayerIndex[PlayerIndexImpacting] = PlayerIndexImpacting2;
	TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacting] = 0;
	
	LastPlayerIndexImpactingPlayerIndex[PlayerIndexImpacting2] = PlayerIndexImpacting;
	TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacting2] = 0;
}

void AEventPush::OnPunch(int PlayerIndexPushing, int PlayerIndexPushed)
{
	if (!bInTickPhase1 && !bInTickPhase2) return;
	if (PlayerIndexPushing == PlayerIndexPushed) return;

	UE_LOG(LogTemp, Warning, TEXT("On Punch, %i, %i"), PlayerIndexPushing, PlayerIndexPushed);
	
	LastPlayerIndexPunchingPlayerIndex[PlayerIndexPushed] = PlayerIndexPushing;
	TimeSinceLastPlayerIndexPunched[PlayerIndexPushed] = 0;
}

void AEventPush::CheckAddScoreOnDeath(int PlayerIndexDeath)
{
	if (!bInTickPhase1 && !bInTickPhase2) return;

	UE_LOG(LogTemp, Warning, TEXT("Check add score on death"));
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("check death zone player : %i"), PlayerIndexDeath));
	
	ScoreSubsystem->AddScore(PlayerIndexDeath, -1);
	
	// Impact
	for (const TTuple<int, int> Element : LastPlayerIndexImpactingPlayerIndex)
	{
		const int PlayerIndexImpacted = Element.Key;
		const int PlayerIndexImpacting = Element.Value;
		
		if (PlayerIndexImpacting != -1 && TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacted] > 0)
		{
			ScoreSubsystem->AddScore(PlayerIndexImpacting, GainScoreOnPush);
		}
	}

	// Punch
	for (const TTuple<int, int> Element : LastPlayerIndexPunchingPlayerIndex)
	{
		const int PlayerIndexPunched = Element.Key;
		const int PlayerIndexPunching = Element.Value;
		
		if (PlayerIndexPunching != -1 && TimeSinceLastPlayerIndexPunched[PlayerIndexPunched] > 0)
		{
			ScoreSubsystem->AddScore(PlayerIndexPunching, GainScoreOnPush);
		}
	}
}


