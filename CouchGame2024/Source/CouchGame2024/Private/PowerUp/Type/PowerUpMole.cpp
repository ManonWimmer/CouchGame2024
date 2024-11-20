// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpMole.h"

#include "Components/SphereComponent.h"
#include "Events/EventData.h"
#include "Events/EventsManager.h"
#include "Kismet/GameplayStatics.h"
#include "Score/GlobalScoreSubsystem.h"


class UGlobalScoreSubsystem;
// Sets default values
APowerUpMole::APowerUpMole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpawnTimeline"));
	InterpFunction.BindUFunction(this, FName("HandleTimelineProgress"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
}

// Called when the game starts or when spawned
void APowerUpMole::BeginPlay()
{
	Super::BeginPlay();

	// Timelines
	InitialZ = GetActorLocation().Z;
	TargetZ = InitialZ + Height; 

	// Data
	if (AEventsManager* EventsManager = Cast<AEventsManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventsManager::StaticClass())))
		MoleToPoints = EventsManager->CurrentEventData->MoleToPoints; // Forcement dans l'event mole
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "MISSING EVENTS MANAGER");
	
	SpawnMole();
}

// Called every frame
void APowerUpMole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpMole::GetPowerUpID() const
{
	return EPowerUpID::Mole;
}

void APowerUpMole::TriggerPowerUp(int PlayerIndex)
{
	UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	if (ScoreSubsystem != nullptr)
	{
		ScoreSubsystem->AddScore(PlayerIndex, MoleToPoints);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow, FString::Printf(TEXT("Add Mole Score to player ID: %i"), PlayerIndex));

		OnMoleCollected.Broadcast();
	}

	DespawnMole();
}

void APowerUpMole::SpawnMole() const
{
	if (SpawnAnimationCurve)
	{
		SpawnTimeline->AddInterpFloat(SpawnAnimationCurve, InterpFunction, FName("Alpha"));
		SpawnTimeline->SetTimelineFinishedFunc(TimelineFinished);
		SpawnTimeline->PlayFromStart();
	}
}

void APowerUpMole::DespawnMole() const
{
	//SphereTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (SpawnAnimationCurve)
	{
		SpawnTimeline->ReverseFromEnd();
	}
}

void APowerUpMole::HandleTimelineProgress(float Value)
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z = FMath::Lerp(InitialZ, TargetZ, Value); 
	SetActorLocation(NewLocation);
}

void APowerUpMole::OnTimelineFinished()
{
	if (SpawnTimeline->GetPlaybackPosition() == 0.0f) // Si c'est le despawn
	{
		Destroy();
	}
}

