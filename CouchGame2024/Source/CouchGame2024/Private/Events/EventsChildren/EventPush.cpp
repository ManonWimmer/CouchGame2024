// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventPush.h"

#include "Events/EventData.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBall/PlayerBall.h"
#include "Score/GlobalScoreSubsystem.h"


// Sets default values
AEventPush::AEventPush()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PushV2Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Project/Assets/Art/03_Mat/Ball/MAT_Ball_Metal.MAT_Ball_Metal"));
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
	if (LastPlayerIndexPushedPlayerIndex.Num() > 0) LastPlayerIndexPushedPlayerIndex.Empty();
	LastPlayerIndexPushedPlayerIndex.Add(0,-1);
	LastPlayerIndexPushedPlayerIndex.Add(1,-1);
	LastPlayerIndexPushedPlayerIndex.Add(2,-1);
	LastPlayerIndexPushedPlayerIndex.Add(3,-1);

	if (TimeSinceLastPlayerIndexPushed.Num() > 0) TimeSinceLastPlayerIndexPushed.Empty();
	TimeSinceLastPlayerIndexPushed.Empty();
	TimeSinceLastPlayerIndexPushed.Add(0,0);
	TimeSinceLastPlayerIndexPushed.Add(1,0);
	TimeSinceLastPlayerIndexPushed.Add(2,0);
	TimeSinceLastPlayerIndexPushed.Add(3,0);
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

	ChangePlayersMaterialsToPushV2();
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
	
	OnPushEndedEvent.Broadcast();

	// Mini timer pour pas qu'on voit la transi aux materials avant l'UI
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,            
		this,                   
		&AEventPush::ResetPlayersMaterialsToInitial, 
		0.25f,                   
		false                   
	);
}

void AEventPush::CheckAddTimeLastPushed(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("CheckAddTimeLastPushed push"));

	UE_LOG(LogTemp, Warning, TEXT("SetupEventPhase1 push - LastPlayerIndexImpactingPlayerIndex: %s"), *FString::JoinBy(LastPlayerIndexPushedPlayerIndex, TEXT(","), [](const TTuple<int, int>& Pair) { return FString::Printf(TEXT("(%d,%d)"), Pair.Key, Pair.Value); }));
	
	for (const TTuple<int, int> Element : LastPlayerIndexPushedPlayerIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("Foreach LastPlayerIndexImpactingPlayerIndex"));
		const int PlayerIndexImpacted = Element.Key;
		const int PlayerIndexImpacting = Element.Value;
		UE_LOG(LogTemp, Warning, TEXT("Impacted : %i, Impacting : %i"), PlayerIndexImpacted, PlayerIndexImpacting);

		if (PlayerIndexImpacting != -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerImpacting != -1"));
			// Add time
			TimeSinceLastPlayerIndexPushed[PlayerIndexImpacted] += DeltaTime;

			// Time Limit
			if (TimeSinceLastPlayerIndexPushed[PlayerIndexImpacted] > TimePushedLimit)
			{
				LastPlayerIndexPushedPlayerIndex[PlayerIndexImpacted] = -1;
				LastPlayerIndexPushedPlayerIndex[PlayerIndexImpacting] = -1;
				TimeSinceLastPlayerIndexPushed[PlayerIndexImpacted] = 0;
			}
		}
	}
}

void AEventPush::OnImpact(int PlayerIndexImpacting, int PlayerIndexImpacting2)
{
	if (!bInTickPhase1 && !bInTickPhase2) return;
	if (PlayerIndexImpacting == PlayerIndexImpacting2) return;

	UE_LOG(LogTemp, Warning, TEXT("On Impact, %i, %i"), PlayerIndexImpacting, PlayerIndexImpacting2);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("On Impact : %i, %i"), PlayerIndexImpacting, PlayerIndexImpacting2));
	
	LastPlayerIndexPushedPlayerIndex[PlayerIndexImpacting] = PlayerIndexImpacting2;
	TimeSinceLastPlayerIndexPushed[PlayerIndexImpacting] = 0;
	
	LastPlayerIndexPushedPlayerIndex[PlayerIndexImpacting2] = PlayerIndexImpacting;
	TimeSinceLastPlayerIndexPushed[PlayerIndexImpacting2] = 0;
}

void AEventPush::OnPunch(int PlayerIndexPushing, int PlayerIndexPushed)
{
	if (!bInTickPhase1 && !bInTickPhase2) return;
	if (PlayerIndexPushing == PlayerIndexPushed) return;

	UE_LOG(LogTemp, Warning, TEXT("On Punch, %i, %i"), PlayerIndexPushing, PlayerIndexPushed);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("On Punch : %i, %i"), PlayerIndexPushing, PlayerIndexPushed));
	
	LastPlayerIndexPushedPlayerIndex[PlayerIndexPushed] = PlayerIndexPushing;
	TimeSinceLastPlayerIndexPushed[PlayerIndexPushed] = 0;
}

void AEventPush::CheckAddScoreOnDeath(int PlayerIndexDeath)
{
	if (!bInTickPhase1 && !bInTickPhase2) return;

	UE_LOG(LogTemp, Warning, TEXT("Check add score on death"));
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("Remove 1 score : %i"), PlayerIndexDeath));
	ScoreSubsystem->AddScore(PlayerIndexDeath, LoseScoreOnDeathZone);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("Time last push : %f"), TimeSinceLastPlayerIndexPushed[PlayerIndexDeath]));
	if (TimeSinceLastPlayerIndexPushed[PlayerIndexDeath] > 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("Index last pushing : %i"), LastPlayerIndexPushedPlayerIndex[PlayerIndexDeath]));
		ScoreSubsystem->AddScore(LastPlayerIndexPushedPlayerIndex[PlayerIndexDeath], GainScoreOnPush);
	}
}

void AEventPush::SetEventData(const UEventData* Data)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, "Set Push Data");
	
	TimePushedLimit = Data->TimePushedLimit;
	LoseScoreOnDeathZone = Data->LoseScoreOnDeathZone;
	GainScoreOnPush = Data->GainScoreOnPush;
}

void AEventPush::ChangePlayersMaterialsToPushV2()
{
	if (Players.Num() == 0) GetAllPlayers();

	for (const TObjectPtr<APlayerBall> Player : Players)
	{
		PlayersInitialMaterials.Add(Player->PlayerIndex, Player->SphereMesh->GetMaterial(0));
		Player->SphereMesh->SetMaterial(0, PushV2Material);
		Player->ReceiveOnSetToNeutralBall();
	}
}

void AEventPush::ResetPlayersMaterialsToInitial()
{
	if (PlayersInitialMaterials.Num() <= 0) return;
	
	for (const TObjectPtr<APlayerBall> Player : Players)
	{
		if (*PlayersInitialMaterials.Find(Player->PlayerIndex))
		{
			Player->SphereMesh->SetMaterial(0, *PlayersInitialMaterials.Find(Player->PlayerIndex));
			Player->ReceiveOnUnsetToNeutralBall();
		}
	}

	PlayersInitialMaterials.Reset();
}

void AEventPush::GetAllPlayers()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, "Get all players from event push");

	TArray<TObjectPtr<AActor>> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBall::StaticClass(), Actors);

	for (TObjectPtr<AActor> Actor : Actors)
	{
		const TObjectPtr<APlayerBall> PlayerBall = Cast<APlayerBall>(Actor);
		Players.Add(PlayerBall);
	}
}


