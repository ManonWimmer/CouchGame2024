// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/MoleElement.h"

#include "Events/EventData.h"
#include "Events/EventsManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBall/PlayerBall.h"
#include "Score/GlobalScoreSubsystem.h"


// Sets default values
AMoleElement::AMoleElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Timelines
	SpawnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpawnTimeline"));
	InterpFunction.BindUFunction(this, FName("HandleTimelineProgress"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
}

// Called when the game starts or when spawned
void AMoleElement::BeginPlay()
{
	Super::BeginPlay();

	// Timelines
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - Height));
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
void AMoleElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMoleElement::TriggerElementWithPlayer(APlayerBall* InPlayerBall)
{
	ReceiveImpactedByPlayer();
	
	if (bHasBeenHitByPlayer) return;
	bHasBeenHitByPlayer = true;
	
	Super::TriggerElementWithPlayer(InPlayerBall);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "Hit mole");

	UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	if (ScoreSubsystem != nullptr)
	{
		ScoreSubsystem->AddScore(InPlayerBall->PlayerIndex, MoleToPoints);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Yellow, FString::Printf(TEXT("Add Mole Score to player ID: %i"), InPlayerBall->PlayerIndex));

		OnMoleCollected.Broadcast();
	}

	DespawnMole();

	BumpEffect();
}

EPinballElementID AMoleElement::GetElementID()
{
	return EPinballElementID::Mole;
}

void AMoleElement::SpawnMole()
{
	if (SpawnAnimationCurve)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Yellow, FString::Printf(TEXT("Spawn mole: %s"), *this->GetName()));

		ReceiveOnSpawnMole();
		
		SpawnTimeline->AddInterpFloat(SpawnAnimationCurve, InterpFunction, FName("Alpha"));
		SpawnTimeline->SetTimelineFinishedFunc(TimelineFinished);
		SpawnTimeline->PlayFromStart();
	}
}

void AMoleElement::DespawnMole()
{
	//SphereTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (SpawnAnimationCurve)
	{
		SpawnTimeline->ReverseFromEnd();
	}

	OnMoleDespawn.Broadcast();
	
	ReceiveOnDespawnMole();
}

void AMoleElement::HandleTimelineProgress(float Value)
{
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = FMath::Lerp(InitialZ, TargetZ, Value); 
		SetActorLocation(NewLocation);
}

void AMoleElement::OnTimelineFinished()
{
	if (SpawnTimeline->GetPlaybackPosition() == 0.0f) // Si c'est le despawn
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Yellow, FString::Printf(TEXT("Destroy mole: %s"), *this->GetName()));
		Destroy();
	}
}

void AMoleElement::BumpEffect()
{
}

