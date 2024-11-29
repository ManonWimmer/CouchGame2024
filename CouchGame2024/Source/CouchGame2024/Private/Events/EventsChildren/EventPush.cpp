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
	Super::SetupEventPhase1();

	#pragma region Reset / Set TMaps
	LastPlayerIndexImpactingPlayerIndex.Reset();
	LastPlayerIndexImpactingPlayerIndex[0] = -1;
	LastPlayerIndexImpactingPlayerIndex[1] = -1;
	LastPlayerIndexImpactingPlayerIndex[2] = -1;
	LastPlayerIndexImpactingPlayerIndex[3] = -1;

	TimeSinceLastPlayerIndexImpacted.Reset();
	LastPlayerIndexImpactingPlayerIndex[0] = 0;
	LastPlayerIndexImpactingPlayerIndex[1] = 0;
	LastPlayerIndexImpactingPlayerIndex[2] = 0;
	LastPlayerIndexImpactingPlayerIndex[3] = 0;

	LastPlayerIndexPunchingPlayerIndex.Reset();
	LastPlayerIndexPunchingPlayerIndex[0] = -1;
	LastPlayerIndexPunchingPlayerIndex[1] = -1;
	LastPlayerIndexPunchingPlayerIndex[2] = -1;
	LastPlayerIndexPunchingPlayerIndex[3] = -1;

	TimeSinceLastPlayerIndexPunched.Reset();
	TimeSinceLastPlayerIndexPunched[0] = 0;
	TimeSinceLastPlayerIndexPunched[1] = 0;
	TimeSinceLastPlayerIndexPunched[2] = 0;
	TimeSinceLastPlayerIndexPunched[3] = 0;
	#pragma endregion
}

void AEventPush::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();

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

void AEventPush::OnImpact(int PlayerIndexImpacting, int PlayerIndexImpacting2)
{
	LastPlayerIndexImpactingPlayerIndex[PlayerIndexImpacting] = PlayerIndexImpacting2;
	TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacting] = 0;
	
	LastPlayerIndexImpactingPlayerIndex[PlayerIndexImpacting2] = PlayerIndexImpacting;
	TimeSinceLastPlayerIndexImpacted[PlayerIndexImpacting2] = 0;
}

void AEventPush::CheckAddTimeLastPushed(float DeltaTime)
{
	// Impact
	for (const TTuple<int, int> Element : LastPlayerIndexImpactingPlayerIndex)
	{
		const int PlayerIndexImpacted = Element.Key;
		const int PlayerIndexImpacting = Element.Value;

		if (PlayerIndexImpacting != -1)
		{
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
		const int PlayerIndexPunched = Element.Key;
		const int PlayerIndexPunching = Element.Value;

		if (PlayerIndexPunching != -1)
		{
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

void AEventPush::OnPunch(int PlayerIndexPushing, int PlayerIndexPushed)
{
	LastPlayerIndexPunchingPlayerIndex[PlayerIndexPushed] = PlayerIndexPushing;
	TimeSinceLastPlayerIndexPunched[PlayerIndexPushed] = 0;
}

void AEventPush::CheckAddScoreOnDeath(int PlayerIndexDeath)
{
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


