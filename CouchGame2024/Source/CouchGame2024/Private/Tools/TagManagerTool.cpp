// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/TagManagerTool.h"

#include "EngineUtils.h"
#include "Selection.h"


// Sets default values
UATagManagerTool::UATagManagerTool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	// Initialisation des tags par défaut
	AvailableTags = { "Minigame1", "Minigame2", "Minigame3" };
}

// Called when the game starts or when spawned


// Called every frame

void UATagManagerTool::AssignTag(FName TagName)
{
	
	if (GEditor)
	{
		// Obtenir les acteurs sélectionnés dans l'éditeur
		TArray<AActor*> SelectedActors;
		GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

		// Assigner le tag à chaque acteur
		for (AActor* Actor : SelectedActors)
		{
			if (Actor && !Actor->Tags.Contains(TagName))
			{
				Actor->Tags.Add(TagName);
				UE_LOG(LogTemp, Log, TEXT("Tag '%s' assigné à %s"), *TagName.ToString(), *Actor->GetName());
			}
		}
	}
}

void UATagManagerTool::RemoveTag(FName TagName)
{
	if (GEditor)
	{
		// Obtenir les acteurs sélectionnés dans l'éditeur
		TArray<AActor*> SelectedActors;
		GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

		// Assigner le tag à chaque acteur
		for (AActor* Actor : SelectedActors)
		{
			if (Actor && Actor->Tags.Contains(TagName))
			{
				Actor->Tags.Remove(TagName);
				UE_LOG(LogTemp, Log, TEXT("Tag '%s' retiré à %s"), *TagName.ToString(), *Actor->GetName());
			}
		}
	}
}

void UATagManagerTool::ShowObjectsWithTag(FName TagName)
{
	TArray<AActor*> Actors = GetActorsWithTag(TagName);
	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(false);
			UE_LOG(LogTemp, Log, TEXT("%s est maintenant visible."), *Actor->GetName());
		}
	}
}

void UATagManagerTool::HideObjectsWithTag(FName TagName)
{
	TArray<AActor*> Actors = GetActorsWithTag(TagName);
	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(true);
			UE_LOG(LogTemp, Log, TEXT("%s est maintenant caché."), *Actor->GetName());
		}
	}
}

TArray<AActor*> UATagManagerTool::GetActorsWithTag(FName TagName)
{
	TArray<AActor*> TaggedActors;

	// Parcourir tous les acteurs dans le niveau
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->Tags.Contains(TagName))
		{
			TaggedActors.Add(Actor);
		}
	}

	return TaggedActors;
}

