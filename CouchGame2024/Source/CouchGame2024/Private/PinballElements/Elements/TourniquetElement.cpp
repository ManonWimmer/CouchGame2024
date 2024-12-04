// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/TourniquetElement.h"

#include "Components/SphereComponent.h"
#include "PinballElements/Data/TourniquetElementData.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "Rounds/RoundsSubsystem.h"


// Sets default values
ATourniquetElement::ATourniquetElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SphereTrigger = CreateDefaultSubobject<USphereComponent>("SphereTrigger");
	
	TourniquetAnchorTurning = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorTurning");
	
	TourniquetAnchorPlayer0 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer0");
	TourniquetAnchorPlayer1 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer1");
	TourniquetAnchorPlayer2 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer2");
	TourniquetAnchorPlayer3 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer3");

	RootComponent = SphereTrigger;
	
	TourniquetAnchorTurning->SetupAttachment(SphereTrigger);

	TourniquetAnchorPlayer0->SetupAttachment(TourniquetAnchorTurning);
	TourniquetAnchorPlayer1->SetupAttachment(TourniquetAnchorTurning);
	TourniquetAnchorPlayer2->SetupAttachment(TourniquetAnchorTurning);
	TourniquetAnchorPlayer3->SetupAttachment(TourniquetAnchorTurning);
}

// Called when the game starts or when spawned
void ATourniquetElement::BeginPlay()
{
	Super::BeginPlay();

	InitDataTourniquet();
	InitAssociatedBallInTourniquetArray();
	
	InitResetable();
}

// Called every frame
void ATourniquetElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleTurningEffect(DeltaTime);

	HandleTimeInTourniquet(DeltaTime);
}

EPinballElementID ATourniquetElement::GetElementID()
{
	return EPinballElementID::Tourniquet;
}

void ATourniquetElement::TriggerElement()
{
	Super::TriggerElement();

	
}

void ATourniquetElement::TriggerElementWithPlayer(APlayerBall* InPlayerBall)
{
	Super::TriggerElementWithPlayer(InPlayerBall);

	AddBallToTourniquet(InPlayerBall);
}

void ATourniquetElement::AddBallToTourniquet(APlayerBall* InPlayerBall)
{
	if (InPlayerBall == nullptr)	return;
	if (InPlayerBall->SphereCollision == nullptr)	return;
	
	
	if (InPlayerBall->BehaviorElementReactions == nullptr)	return;
	
	int IndexToAddBall = GetNextEmptyBallPositionTourniquet();

	if (IndexToAddBall == -1)
		return;

	if (IndexToAddBall >= AssociatedBallInTourniquet.Num())
		return;

	
	AssociatedBallInTourniquet[IndexToAddBall] = InPlayerBall;

	USceneComponent* OutTourniquetAnchor = GetTourniquetAnchorFromIndex(IndexToAddBall);
	
	if (OutTourniquetAnchor == nullptr)	return;

	float TempVelocityLength = InPlayerBall->GetVelocity().Length();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Ball speed in tourniquet : %f"), TempVelocityLength));
	
	InPlayerBall->BehaviorElementReactions->ReceiveTourniquetReaction();
	InPlayerBall->SphereCollision->AttachToComponent(OutTourniquetAnchor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	AssociatedBallInTourniquet[IndexToAddBall] = InPlayerBall;
	AssociatedTimeToBallInTourniquet[IndexToAddBall] = 0.f;
	
	IncreaseSpeedTourniquetByVelocity(TempVelocityLength);
}

void ATourniquetElement::InitResetable()
{
	if (!GetWorld())	return;

	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr)	return;

	RoundsSubsystem->AddResetableObject(this);
}

bool ATourniquetElement::IsResetable()
{
	return true;
}

void ATourniquetElement::ResetObject()
{
	for (APlayerBall* InPlayerBall: AssociatedBallInTourniquet)
	{
		if (InPlayerBall == nullptr)	continue;
		InPlayerBall->SphereCollision->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	InitAssociatedBallInTourniquetArray();
}

void ATourniquetElement::ForceExpulsePlayerFromTourniquetWithoutIndex(APlayerBall* InPlayerBall)	// expulse method called from player manually exit tourniquet
{
	if (InPlayerBall == nullptr)	return;
	
	int IndexPlayer = GetPlayerIndexFromTourniquet(InPlayerBall);

	if (IndexPlayer <= -1)	return;

	if (AssociatedTimeToBallInTourniquet.Num() <= IndexPlayer)	return;

	InPlayerBall->BehaviorElementReactions->bUseForceExitTourniquet = true;
	
	AssociatedTimeToBallInTourniquet[IndexPlayer] = TurningDuration * 2.f;
}

void ATourniquetElement::ExpulsePlayerFromTourniquet(APlayerBall* InPlayerBall, int Index)	// expulse method called from tourniquet manually expulsing player
{
	if (InPlayerBall == nullptr)	return;
	if (InPlayerBall->SphereCollision == nullptr)	return;
	if (InPlayerBall->BehaviorElementReactions == nullptr)	return;
	
	InPlayerBall->SphereCollision->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	InPlayerBall->BehaviorElementReactions->ReceiveEndTourniquetReaction();

	USceneComponent* OutAnchor = GetTourniquetAnchorFromIndex(Index);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Expulse from tourniquet");

	if (InPlayerBall->BehaviorElementReactions->bUseForceExitTourniquet)
	{
		FVector Dir = InPlayerBall->BehaviorElementReactions->GetCurrentForceExitTourniquetDir();
		InPlayerBall->SphereCollision->AddImpulse(Dir * GetForceExpulseBall());
	}
	else
	{
		InPlayerBall->SphereCollision->AddImpulse(OutAnchor->GetRightVector() * GetForceExpulseBall());
	}

	InPlayerBall->BehaviorElementReactions->bUseForceExitTourniquet = false;
}

void ATourniquetElement::IncreaseSpeedTourniquetByVelocity(float InVelocity)
{
	if (InVelocity <= 0)	return;

	CurrentTurningSpeed += InVelocity * 1.f;
}

int ATourniquetElement::GetNextEmptyBallPositionTourniquet()
{
	for (int i = 0; i < AssociatedBallInTourniquet.Num(); ++i)
	{
		if (AssociatedBallInTourniquet[i] == nullptr)	return i;
	}

	return -1;
}

USceneComponent* ATourniquetElement::GetTourniquetAnchorFromIndex(int InIndex)
{
	if (InIndex >= 4 || InIndex < 0)
		return nullptr;

	switch (InIndex)
	{
		case 0:
			return TourniquetAnchorPlayer0;
		case 1:
			return TourniquetAnchorPlayer1;
		case 2:
			return TourniquetAnchorPlayer2;
		case 3:
			return TourniquetAnchorPlayer3;

		default:
			break;
	}

	return nullptr;
}

int ATourniquetElement::GetPlayerIndexFromTourniquet(APlayerBall* InPlayerBall)
{
	if (InPlayerBall == nullptr)	return -1;

	for (int i = 0; i < AssociatedBallInTourniquet.Num(); ++i)
	{
		if (AssociatedBallInTourniquet[i] == nullptr)	continue;

		if (AssociatedBallInTourniquet[i] == InPlayerBall)	return i;
	}

	return -1;
}

void ATourniquetElement::InitAssociatedBallInTourniquetArray()
{
	AssociatedBallInTourniquet.Empty();

	for (int i = 0; i < 4; ++i)
	{
		AssociatedBallInTourniquet.Add(nullptr);
		AssociatedTimeToBallInTourniquet.Add(0.f);
	}
}

void ATourniquetElement::InitDataTourniquet()
{
	if (TourniquetData == nullptr)	return;
	
	TurningDuration = TourniquetData->TurningDuration;

	MinTurningTurningSpeed = TourniquetData->MinTurningTurningSpeed;
	MaxTurningTurningSpeed = TourniquetData->MaxTurningTurningSpeed;

	MinExpulseForce = TourniquetData->MinExpulseForce;
	MaxExpulseForce = TourniquetData->MaxExpulseForce;
}

float ATourniquetElement::GetForceExpulseBall()
{
	float Percent = (CurrentTurningSpeed - MinTurningTurningSpeed)/(MaxTurningTurningSpeed - MinTurningTurningSpeed);

	Percent = FMath::Clamp(Percent, 0.f, 1.f);

	float OutExpulseForce = FMath::Lerp(MinExpulseForce, MaxExpulseForce, Percent);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("PercentForce : %f"), Percent));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("CurrentSpeed : %f"), CurrentTurningSpeed));
	
	return OutExpulseForce;
}

void ATourniquetElement::HandleTurningEffect(float DeltaTime)
{
	if (TourniquetAnchorTurning == nullptr)	return;

	CurrentTurningSpeed = FMath::Clamp(CurrentTurningSpeed, MinTurningTurningSpeed, MaxTurningTurningSpeed);
	
	FRotator RotationToAdd = FRotator(0, CurrentTurningSpeed, 0);

	HandleDecreaseSpeedEffect(DeltaTime);
	
	TourniquetAnchorTurning->AddLocalRotation(RotationToAdd * DeltaTime);
}

void ATourniquetElement::HandleDecreaseSpeedEffect(float DeltaTime)
{
	CurrentTurningSpeed -= DeltaTime * TurningSpeedDecceleration;

	CurrentTurningSpeed = FMath::Clamp(CurrentTurningSpeed, MinTurningTurningSpeed, MaxTurningTurningSpeed);
}

void ATourniquetElement::HandleTimeInTourniquet(float DeltaTime)
{
	for (int i = 0; i < AssociatedTimeToBallInTourniquet.Num(); i++)
	{
		if (i >= AssociatedBallInTourniquet.Num())	continue;
		if (i >= AssociatedTimeToBallInTourniquet.Num())	continue;
		
		if (AssociatedTimeToBallInTourniquet[i] > TurningDuration)
		{
			if (AssociatedBallInTourniquet[i] == nullptr)
				continue;

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Turning ball Index %f"), i));
			
			ExpulsePlayerFromTourniquet(AssociatedBallInTourniquet[i], i);
			AssociatedBallInTourniquet[i] = nullptr;
		}
		else
		{
			if (AssociatedBallInTourniquet[i] == nullptr)
				continue;
			
			AssociatedTimeToBallInTourniquet[i] += DeltaTime;
		}
	}
}
