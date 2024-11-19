// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/TagManagerTool.h"

#include "EngineUtils.h"
#include "Selection.h"


// Sets default values
UATagManagerTool::UATagManagerTool()
{
}

// Called when the game starts or when spawned


// Called every frame

void UATagManagerTool::AssignTag(FName TagName)
{
	for (FName Tag : AvailableTags)
	{
		if (Tag != TagName)
		{
			RemoveTag(Tag);
		}
	}
	
	if (GEditor)
	{
		// Get selected actors in editor
		TArray<AActor*> SelectedActors;
		GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

		// Add tag to each actor
		for (AActor* Actor : SelectedActors)
		{
			if (Actor && !Actor->Tags.Contains(TagName))
			{
				Actor->Tags.Add(TagName);
				UE_LOG(LogTemp, Log, TEXT("Tag '%s' added to %s"), *TagName.ToString(), *Actor->GetName());
			}
		}
	}
}

void UATagManagerTool::RemoveTag(FName TagName)
{
	if (GEditor)
	{
		// Get selected actors in editor
		TArray<AActor*> SelectedActors;
		GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

		// Remove tag from each actor
		for (AActor* Actor : SelectedActors)
		{
			if (Actor && Actor->Tags.Contains(TagName))
			{
				Actor->Tags.Remove(TagName);
				UE_LOG(LogTemp, Log, TEXT("Tag '%s' removed from %s"), *TagName.ToString(), *Actor->GetName());
			}
		}
	}
}

void UATagManagerTool::ShowObjectsWithTag(FName TagName)
{
	HideOtherTags(TagName);
	
	TArray<AActor*> Actors = GetActorsWithTag(TagName);
	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			Actor->SetIsTemporarilyHiddenInEditor(false);
			UE_LOG(LogTemp, Log, TEXT("%s set visible to true."), *Actor->GetName());
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
			Actor->SetIsTemporarilyHiddenInEditor(true);
			UE_LOG(LogTemp, Log, TEXT("%s set visible to false."), *Actor->GetName());
		}
	}
}

void UATagManagerTool::HideOtherTags(FName TagName)
{
	for (FName Tag : AvailableTags)
	{
		if (Tag != TagName && Tag != "Constant")
		{
			HideObjectsWithTag(Tag);
		}
	}
}

void UATagManagerTool::SetMissingTagsToConstant()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && HasTagInList(Actor) == "")
		{
			Actor->Tags.Add("Constant");
			UE_LOG(LogTemp, Log, TEXT("%s missing tag, add constant."), *Actor->GetName());
		}
	}
}

void UATagManagerTool::ShowAll()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor)
		{
			Actor->SetIsTemporarilyHiddenInEditor(false);
		}
	}
}


void UATagManagerTool::RemoveAllTags()
{
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && HasTagInList(Actor) != "")
		{
			Actor->Tags.Remove(HasTagInList(Actor));
		}
	}
}

TArray<AActor*> UATagManagerTool::GetActorsWithTag(FName TagName) const
{
	TArray<AActor*> TaggedActors;
	
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

FName UATagManagerTool::HasTagInList(AActor* Actor)
{
	if (!Actor) return "";

	// Iterate over the tag list
	for (FName Tag : AvailableTags)
	{
		if (Actor->ActorHasTag(Tag))
		{
			return Tag; // Found a matching tag
		}
	}

	return ""; // No matching tag found
}

