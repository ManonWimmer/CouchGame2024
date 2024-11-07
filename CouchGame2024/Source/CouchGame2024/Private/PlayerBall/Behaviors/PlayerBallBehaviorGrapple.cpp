// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"

#include "Components/SphereComponent.h"
#include "GrapplingHook/Hookable.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallController.h"
#include "PlayerBall/Datas/PlayerBallData.h"
#include "GrapplingHook/Hookable.h"

UPlayerBallBehaviorGrapple::UPlayerBallBehaviorGrapple()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerBallBehaviorGrapple::BeginPlay()
{
	Super::BeginPlay();
}


void UPlayerBallBehaviorGrapple::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerBallBehaviorGrapple::InitBehavior()
{
	Super::InitBehavior();
}

void UPlayerBallBehaviorGrapple::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	if (GetPlayerBallController() == nullptr) return;

	GetPlayerBallController()->OnPlayerGrapplingInputStarted.AddDynamic(
		this, &UPlayerBallBehaviorGrapple::ReceiveGrapplingActionStarted);
	GetPlayerBallController()->OnPlayerGrapplingInputEnded.AddDynamic(
		this, &UPlayerBallBehaviorGrapple::ReceiveGrapplingActionEnded);
	GetPlayerBallController()->OnPlayerMoreLessGrapplingInput.AddDynamic(
		this, &UPlayerBallBehaviorGrapple::MoreLessAction);
}

void UPlayerBallBehaviorGrapple::SetupData()
{
	Super::SetupData();

	if (GetPlayerBall() == nullptr) return;
	if (GetPlayerBall()->GetPlayerBallData() == nullptr) return;

	// Grappling
	GrapplingDamping = GetPlayerBall()->GetPlayerBallData()->GrapplingDamping;
	GrapplingForce = GetPlayerBall()->GetPlayerBallData()->GrapplingForce;
	GrapplingReleaseForce = GetPlayerBall()->GetPlayerBallData()->GrapplingReleaseForce;
	MinCableDistance = GetPlayerBall()->GetPlayerBallData()->MinCableDistance;
	MaxCableDistance = GetPlayerBall()->GetPlayerBallData()->MaxCableDistance;
	MoreOrLessCablePerFrame = GetPlayerBall()->GetPlayerBallData()->MoreOrLessCablePerFrame;
	StartGrapplingForceFactorWhenAlreadyMoving = GetPlayerBall()->GetPlayerBallData()->
	                                                              StartGrapplingForceFactorWhenAlreadyMoving;
	GetPlayerBall()->GrapplingSphereCollision->SetSphereRadius(MaxCableDistance); // Max grappling cable distance
}

void UPlayerBallBehaviorGrapple::ReceiveGrapplingActionStarted(float InGrapplingValue)
{
	if (!CanGrappling)
		return;

	if (GetPlayerBall() == nullptr) return;

	GrapplingValue = InGrapplingValue;

	IsGrappling = false;


	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- // 
	/* 
	TArray<TObjectPtr<AActor>> OverlappingActors;
	TArray<TObjectPtr<APlayerBall>> OverlappingPlayers;
	GetPlayerBall()->SphereCollision->GetOverlappingActors(OverlappingActors, APlayerBall::StaticClass());
	
	// Return if none
	if (OverlappingActors.Num() <= 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "pas de joueur");
		return;
	}

	// For each player in overlap radius
	for (TObjectPtr<AActor> Actor : OverlappingActors)
	{
		// Check if different from self
		if (Actor != GetPlayerBall())
		{
			TObjectPtr<APlayerBall> OtherPlayer = Cast<APlayerBall>(Actor);
			if (OtherPlayer)
			{
				// Check if both can grapple & be grappled
				if (OtherPlayer->BehaviorGrapple != nullptr)
				{
					if (OtherPlayer->BehaviorGrapple->GrappledPlayerBall == nullptr && OtherPlayer->BehaviorGrapple->
						GrapplingPlayerBall == nullptr && OtherPlayer->BehaviorGrapple->CanBeGrappled)
						OverlappingPlayers.Add(OtherPlayer);
				}
			}
		}
	}

	// Return if no player in grappling radius
	if (OverlappingPlayers.Num() <= 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "pas de joueur 2");
		return;
	}

	// Get nearest player in radius between all overlapping
	TObjectPtr<APlayerBall> NearestPlayerBall = OverlappingPlayers[0];
	float NearestDistance = FVector::Dist(GetPlayerBall()->GetActorLocation(), NearestPlayerBall->GetActorLocation());

	for (APlayerBall* OutPlayerBall : OverlappingPlayers)
	{
		float NewNearestDistance =
			FVector::Dist(GetPlayerBall()->GetActorLocation(), OutPlayerBall->GetActorLocation());
		if (NewNearestDistance < NearestDistance)
		{
			NearestPlayerBall = OutPlayerBall;
			NearestDistance = NewNearestDistance;
		}
	}

	GrappledPlayerBall = NearestPlayerBall;

	if (GrappledPlayerBall == nullptr)
		return;

	// UE_LOG(LogTemp, Warning, TEXT("Set grappledPlayerBall") );
	// UE_LOG(LogTemp, Log, TEXT("Current State : %hhd"), (StateMachine->GetCurrentStateID()) );

	if (GrappledPlayerBall->BehaviorGrapple == nullptr) return;

	GrappledPlayerBall->BehaviorGrapple->GrapplingPlayerBall = GetPlayerBall();

	// UE_LOG(LogTemp, Warning, TEXT("Set grapplingPlayerBall") );
	// UE_LOG(LogTemp, Log, TEXT("Current State : %hhd"), (StateMachine->GetCurrentStateID()) );

	IsGrappling = true;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "broadcast grappling");
	*/
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	TArray<TObjectPtr<AActor>> OverlappingActors;
	TArray<TObjectPtr<UObject>> OverlappingHookObjects;
	TArray<TScriptInterface<IHookable>> OverlappingHookInterfaces;
	GetPlayerBall()->GrapplingSphereCollision->GetOverlappingActors(OverlappingActors);

	// Return if none
	if (OverlappingActors.Num() <= 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "1 - pas de hook point");
		return;
	}

	// For each player in overlap radius, add to overlapping hook objects if implements interface hookable
	for (TObjectPtr<AActor> Actor : OverlappingActors)
	{
		FString ActorName = Actor->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Actor Name: %s"), *ActorName));

		if (Actor != nullptr && Actor->Implements<UHookable>())
		{
			TScriptInterface<IHookable> Interface = Actor;

			// Check if is hookable even if implements interface
			if (Interface->IsHookable())
			{
				OverlappingHookObjects.Add(Actor);
				OverlappingHookInterfaces.Add(Actor);
			}
		}
	}

	// Return if no hook point in grappling radius
	if (OverlappingHookObjects.Num() <= 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "2 - pas de hook point");
		return;
	}

	// Get nearest hook point in radius between all overlapping
	TObjectPtr<UObject> NearestHookObject = OverlappingHookObjects[0];
	TScriptInterface<IHookable> NearestHookInterface = OverlappingHookInterfaces[0];
	float NearestDistance = FVector::Dist(GetPlayerBall()->GetActorLocation(), NearestHookInterface->GetHookPosition());

	for (UObject* OutHookObject : OverlappingHookObjects)
	{
		float NewNearestDistance =
			FVector::Dist(GetPlayerBall()->GetActorLocation(), NearestHookInterface->GetHookPosition());
		if (NewNearestDistance < NearestDistance)
		{
			NearestHookObject = OutHookObject;
			NearestHookInterface = OutHookObject;
			NearestDistance = NewNearestDistance;
		}
	}

	if (NearestHookObject == nullptr || NearestHookInterface == nullptr) return;

	HookObject = NearestHookObject;
	HookInterface = NearestHookInterface;

	UE_LOG(LogTemp, Warning, TEXT("Set NearestHookObject"));
	// UE_LOG(LogTemp, Log, TEXT("Current State : %hhd"), (StateMachine->GetCurrentStateID()) );

	IsGrappling = true;

	IsHookingPillar = HookInterface->IsPillar();
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "broadcast grappling");
	OnGrapplingActionStarted.Broadcast(GrapplingValue);
}

void UPlayerBallBehaviorGrapple::ReceiveGrapplingActionEnded(float InGrapplingValue)
{
	OnGrapplingActionEnded.Broadcast(InGrapplingValue);
}

void UPlayerBallBehaviorGrapple::ReceiveGrappledActionStarted(float InGrappledValue)
{
	if (!CanBeGrappled) return;

	OnGrappledActionStarted.Broadcast(InGrappledValue);
}

void UPlayerBallBehaviorGrapple::ReceiveGrappledActionEnded(float InGrappledValue)
{
	OnGrappledActionEnded.Broadcast(InGrappledValue);
}

void UPlayerBallBehaviorGrapple::MoreLessAction(float InMoreLessValue)
{
	MoreLessValue = InMoreLessValue;
}
