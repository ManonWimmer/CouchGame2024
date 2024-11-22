// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/Duck/DuckBank.h"

#include "Components/SphereComponent.h"
#include "Events/EventData.h"
#include "Events/EventsManager.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBall/PlayerBall.h"
#include "Score/GlobalScoreSubsystem.h"


class AEventsManager;
// Sets default values
ADuckBank::ADuckBank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BankMesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision")); // overlap player balls
	
	RootComponent = SphereCollision;
	BankMesh->SetupAttachment(SphereCollision);
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADuckBank::OnBankBeginOverlap);
}

// Called when the game starts or when spawned
void ADuckBank::BeginPlay()
{
	Super::BeginPlay();

	// Data
	if (AEventsManager* EventsManager = Cast<AEventsManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventsManager::StaticClass())))
		DuckToPoints = EventsManager->CurrentEventData->DuckToPoints; 
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "MISSING EVENTS MANAGER");
	
}

// Called every frame
void ADuckBank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADuckBank::OnBankBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	if (TObjectPtr<APlayerBall> BallInBank = Cast<APlayerBall>(OtherActor))
	{
		UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
		if (ScoreSubsystem != nullptr)
		{
			ScoreSubsystem->PlayerInDuckBank(BallInBank->PlayerIndex, DuckToPoints);
		}
	}
}

