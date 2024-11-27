// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/EventZonesV2Manager.h"

#include "Events/EventsChildren/EventZones.h"
#include "Kismet/GameplayStatics.h"
#include "PinballElements/Elements/PillarElement.h"
#include "Zone/PillarZone.h"


class AEventZones;
// Sets default values
AEventZonesV2Manager::AEventZonesV2Manager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventZonesV2Manager::BeginPlay()
{
	Super::BeginPlay();

	GetPhase2ZonesAndPillars();
}

// Called every frame
void AEventZonesV2Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInPhase2)
	{
		switch (CurrentState) {
		case EPillarManagerState::Waiting:
			Wait(DeltaTime);
			break;
		case EPillarManagerState::Static:
			Static(DeltaTime);
			break;
		}
	}
}

void AEventZonesV2Manager::Bind()
{
	if (!bHasBeenBind)
	{
		if (AEventZones* EventZones = Cast<AEventZones>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventZones::StaticClass())))
		{
			bHasBeenBind = true;
			EventZones->OnZonesStartedEvent.AddDynamic(this, &AEventZonesV2Manager::SetStartValues);
			//EventZones->OnZonesPhase1StartedEvent.AddDynamic(this, &APillarZone::OnStartPhase1);
			EventZones->OnZonesPhase2StartedEvent.AddDynamic(this, &AEventZonesV2Manager::StartPhase2);
			EventZones->OnZonesEndedEvent.AddDynamic(this, &AEventZonesV2Manager::EndPhase2);
		}
		else
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "CANT FIND ZONES EVENT FROM DUCK SPAWNER");
		}
	}
}

void AEventZonesV2Manager::SetStartValues()
{
	SelectedZonesAndPillars.Reset();
	TrickedZoneAndPillar = TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>>(nullptr, nullptr);
}

void AEventZonesV2Manager::GetPhase2ZonesAndPillars()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Phase2Zones");

	// Get Zones Phase 2
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarZone::StaticClass(), Actors);
    
	for (AActor* Actor : Actors)
	{
		if (APillarZone* PillarZone = Cast<APillarZone>(Actor))
		{
			if (PillarZone->PillarZonePhase == EPillarZonePhase::Phase2)
				Phase2Zones.Add(PillarZone);
		}
	}

	// Get Pillars Phase2Pillars
	TArray<AActor*> Actors2;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarElement::StaticClass(), Actors);
    
	for (AActor* Actor : Actors)
	{
		if (APillarElement* Pillar = Cast<APillarElement>(Actor))
		{
			Phase2Pillars.Add(Pillar);
		}
	}
}

void AEventZonesV2Manager::GetRandomPillars()
{
	if (Phase2Zones.Num() == 0 || Phase2Pillars.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No zones or pillars available for selection."));
		return;
	}
	
	for (TObjectPtr<APillarZone> Zone : Phase2Zones)
	{
		if (SelectedZonesAndPillars.Contains(Zone))
		{
			continue; 
		}
		
		TArray<TObjectPtr<APillarElement>> AvailablePillars = Phase2Pillars;

		// Remove already selected pillars
		for (const TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>>& Pair : SelectedZonesAndPillars)
		{
			AvailablePillars.Remove(Pair.Value);
		}

		// Check if still remaining pillars
		if (AvailablePillars.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No more pillars available for selection."));
			return;
		}

		// Get random pillar in remaining
		int32 RandomIndex = FMath::RandRange(0, AvailablePillars.Num() - 1);
		TObjectPtr<APillarElement> SelectedPillar = AvailablePillars[RandomIndex];

		// Add to TMap
		SelectedZonesAndPillars.Add(Zone, SelectedPillar);

		UE_LOG(LogTemp, Log, TEXT("Zone %s assigned to Pillar %s. %s"),
			*Zone->GetName(),
			*SelectedPillar->GetName(),
			*Zone->GetActorLocation().ToString());
	}

	// Get random tricked zone & pillar
	// Temp array from TMap
	TArray<TObjectPtr<APillarZone>> KeysArray;
	SelectedZonesAndPillars.GenerateKeyArray(KeysArray);

	if (KeysArray.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, KeysArray.Num() - 1);
		TObjectPtr<APillarZone> RandomZone = KeysArray[RandomIndex];

		TrickedZoneAndPillar = TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>>(RandomZone, SelectedZonesAndPillars[RandomZone]);
	}
}

void AEventZonesV2Manager::ShowRandomPillars()
{
	for (TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>> ZoneAndPillar : SelectedZonesAndPillars)
	{
		const TObjectPtr<APillarZone> Zone = ZoneAndPillar.Key;
		const TObjectPtr<APillarElement> Pillar = ZoneAndPillar.Value;

		// Set location of zone to pillar location
		FVector TargetLocation = FVector(Pillar->GetActorLocation().X, Pillar->GetActorLocation().Y, Zone->GetActorLocation().Z);
		Zone->SetActorLocation(TargetLocation);

		// Enable zone
		if (Zone == TrickedZoneAndPillar.Key)
		{
			Zone->ShowZone(true);
		}
		else
		{
			Zone->ShowZone(false);
		}
	}
}

void AEventZonesV2Manager::ResetRandomPillars()
{
	for (TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>> ZoneAndPillar : SelectedZonesAndPillars)
	{
		TObjectPtr<APillarZone> Zone = ZoneAndPillar.Key;
		Zone->HideZone();
	}

	SelectedZonesAndPillars.Reset();
	TrickedZoneAndPillar = TTuple<TObjectPtr<APillarZone>, TObjectPtr<APillarElement>>(nullptr, nullptr);
}

void AEventZonesV2Manager::StartPhase2()
{
	bIsInPhase2 = true;
}

void AEventZonesV2Manager::EndPhase2()
{
	bIsInPhase2 = false;
}

void AEventZonesV2Manager::Wait(float DeltaTime)
{
	if (CurrentWaitTime == 0.f)
	{
		CurrentWaitTime = FMath::FRandRange(MinTimeWaitingOnPillar, MaxTimeWaitingOnPillar);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan,
													  FString::Printf(TEXT("Time to wait v2 : %f"), CurrentWaitTime));
	}
	else
	{
		TimeWaited += DeltaTime;

		if (TimeWaited >= CurrentWaitTime)
		{
			GetRandomPillars();
			ShowRandomPillars();
			CurrentState = EPillarManagerState::Static;
			TimeWaited = 0.f;
			CurrentWaitTime = 0.f;
		}
	}
}

void AEventZonesV2Manager::Static(float DeltaTime)
{
	if (CurrentWaitTime == 0.f)
	{
		CurrentWaitTime = FMath::FRandRange(MinTimeStaticOnPillar, MaxTimeStaticOnPillar);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan,
													  FString::Printf(TEXT("Time to wait static v2 : %f"), CurrentWaitTime));
	}
	else
	{
		TimeWaited += DeltaTime;

		if (TimeWaited >= CurrentWaitTime)
		{
			ResetRandomPillars();
			CurrentState = EPillarManagerState::Waiting;
			TimeWaited = 0.f;
			CurrentWaitTime = 0.f;
		}
	}
}

