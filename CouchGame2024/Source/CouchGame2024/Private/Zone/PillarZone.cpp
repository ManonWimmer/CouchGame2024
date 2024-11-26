// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/PillarZone.h"

#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Events/EventsChildren/EventZones.h"
#include "GrapplingHook/Hookable.h"
#include "Kismet/GameplayStatics.h"
#include "PinballElements/Elements/PillarElement.h"


// Sets default values
APillarZone::APillarZone()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up a RootComponent
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create and initialize the SphereComponent
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->InitSphereRadius(200.f);

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APillarZone::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APillarZone::OnOverlapEnd);

	// Create and initialize the SpotLightComponent
	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLightComponent->SetupAttachment(RootComponent);
	SpotLightComponent->SetIntensity(5000.f); 
	SpotLightComponent->SetLightColor(FLinearColor::White);
	SpotLightComponent->SetAttenuationRadius(500.f);
	SpotLightComponent->SetOuterConeAngle(30.f);
	SpotLightComponent->SetInnerConeAngle(15.f);

	// Orient the spotlight to point downward
	SpotLightComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // Pitch -90 degrees to point down
}


// Called when the game starts or when spawned
void APillarZone::BeginPlay()
{
	Super::BeginPlay();
	
	GetLevelPillars();
	
	Bind();
}

// Called every frame
void APillarZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInPhase1 && PillarZonePhase == EPillarZonePhase::Phase1)
	{
		switch (CurrentZoneState) {
		case EPillarZoneState::Waiting:
			Wait(DeltaTime);
			break;
		case EPillarZoneState::Moving:
			MoveToPillar(DeltaTime);
			break;
		}
	}
	else if (bIsInPhase2 && PillarZonePhase == EPillarZonePhase::Phase2)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan,"PillarZone::TickPhase2");
	}
}

void APillarZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (APillarElement* OverlappedPillar = Cast<APillarElement>(OtherActor))
	{
		OverlappedPillar->EnablePillar();
	}
}

void APillarZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (APillarElement* OverlappedPillar = Cast<APillarElement>(OtherActor))
	{
		OverlappedPillar->DisablePillar();
	}
}

void APillarZone::GetLevelPillars()
{
	ScenePillars.Reset();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarElement::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (APillarElement* Pillar = Cast<APillarElement>(Actor))
		{
			ScenePillars.Add(Pillar);
			UE_LOG(LogTemp, Warning, TEXT("Found Pillar: %s"), *Pillar->GetName());
		}
	}
}

void APillarZone::GetRandomPillar()
{
	if (ScenePillars.Num() <= 1) 
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Not enough pillars to choose from"));
		return;
	}

	APillarElement* ExcludedPillar = TargetPillar; 

	// Filter list without excluded pillar
	TArray<APillarElement*> FilteredList = ScenePillars.FilterByPredicate([ExcludedPillar](const APillarElement* Item)
	{
		return Item != ExcludedPillar;
	});

	// Check list
	if (FilteredList.Num() <= 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No valid pillars to choose from!"));
		return;
	}

	// Get random from filtered list
	if (APillarElement* RandomPillar = FilteredList[FMath::RandRange(0, FilteredList.Num() - 1)])
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			FString::Printf(TEXT("New Random Pillar: %s"), *RandomPillar->GetName()));
		
		TargetPillar = RandomPillar;
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to pick a random pillar!"));
	}
}

void APillarZone::MoveToPillar(float DeltaTime)
{
	if (!bIsInPhase1) return;
	
	FVector TargetLocation = FVector(TargetPillar->GetActorLocation().X, TargetPillar->GetActorLocation().Y, GetActorLocation().Z);
	FVector CurrentLocation = GetActorLocation();  
	FVector Direction = TargetLocation - CurrentLocation;  
	float DistanceToMove = MoveSpeed * DeltaTime;  

	// Get closer to target
	if (Direction.Size() > DistanceToMove)
	{
		Direction.Normalize();  
		FVector NewLocation = CurrentLocation + Direction * DistanceToMove; 
		SetActorLocation(NewLocation);  
	}
	else // End movement
	{
		SetActorLocation(TargetLocation);
		CurrentZoneState = EPillarZoneState::Waiting;
	}
}

void APillarZone::Wait(float DeltaTime)
{
	if (!bIsInPhase1) return;
	
	if (CurrentWaitTime == 0.f)
	{
		CurrentWaitTime = FMath::FRandRange(MinTimeWaitingOnPillar, MaxTimeWaitingOnPillar);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red,
		                                              FString::Printf(TEXT("Time to wait : %f"), CurrentWaitTime));
	}
	else
	{
		TimeWaited += DeltaTime;

		if (TimeWaited >= CurrentWaitTime)
		{
			GetRandomPillar();
			CurrentZoneState = EPillarZoneState::Moving;
			TimeWaited = 0.f;
			CurrentWaitTime = 0.f;
		}
	}
}

void APillarZone::OnStartPhase1()
{
	if (PillarZonePhase == EPillarZonePhase::Phase2)
	{
		bIsInPhase1 = false;
		bIsInPhase2 = false;
		return;
	}
	
	bIsInPhase1 = true;
	bIsInPhase2 = false;
	
	ShowZone();
}

void APillarZone::OnEndPhase1AndStartPhase2()
{
	if (PillarZonePhase == EPillarZonePhase::Phase1)
	{
		bIsInPhase1 = false;
		bIsInPhase2 = false;
		return;
	}
	
	bIsInPhase1 = false;
	bIsInPhase2 = true;

	ShowZone();
}

void APillarZone::OnEndPhase2()
{
	bIsInPhase1 = false;
	bIsInPhase2 = false;

	HideZone();
}

void APillarZone::Bind()
{
	if (!bHasBeenBind)
	{
		if (AEventZones* EventZones = Cast<AEventZones>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventZones::StaticClass())))
		{
			bHasBeenBind = true;
			EventZones->OnZonesStartedEvent.AddDynamic(this, &APillarZone::SetStartValues);
			EventZones->OnZonesPhase1StartedEvent.AddDynamic(this, &APillarZone::OnStartPhase1);
			EventZones->OnZonesPhase2StartedEvent.AddDynamic(this, &APillarZone::OnEndPhase1AndStartPhase2);
			EventZones->OnZonesEndedEvent.AddDynamic(this, &APillarZone::OnEndPhase2);
		}
		else
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "CANT FIND ZONES EVENT FROM DUCK SPAWNER");
		}
	}
}

void APillarZone::ShowZone() const
{
	if (SpotLightComponent) SpotLightComponent->SetVisibility(true);
	if (SphereComponent) SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	EnableOverlappingPillars();
}

void APillarZone::HideZone() const
{
	if (SpotLightComponent) SpotLightComponent->SetVisibility(false);
	if (SphereComponent) SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APillarZone::SetStartValues()
{
	bIsInPhase1 = false;
	bIsInPhase2 = false;

	HideZone();

	if (SpawnPillar && PillarZonePhase == EPillarZonePhase::Phase1)
	{
		TargetPillar = SpawnPillar;
		FVector TargetLocation = FVector(TargetPillar->GetActorLocation().X, TargetPillar->GetActorLocation().Y, GetActorLocation().Z);
		SetActorLocation(TargetLocation);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "MISSING SPAWN PILLAR IN PILLAR ZONE");
	}
	
}

void APillarZone::EnableOverlappingPillars() const
{
	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (APillarElement* Pillar = Cast<APillarElement>(Actor))
		{
			Pillar->EnablePillar();
		}
	}
}
