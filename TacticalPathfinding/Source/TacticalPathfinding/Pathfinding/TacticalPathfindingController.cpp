#include "TacticalPathfindingController.h"

UTacticalPathfindingController::UTacticalPathfindingController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UTacticalPathfindingController* UTacticalPathfindingController::FindInstanceInWorld(UWorld* world)
{
	//Get all the actors in the current world:
	TArray<AActor*> actors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(world, AActor::StaticClass(), actors);
	//Find the actor that has an influence map controller attached to it:
	AActor** actor = actors.FindByPredicate([](AActor*& item)
	{
		return item->FindComponentByClass<UTacticalPathfindingController>() != nullptr;
	});
	//Return the influence map controller component:
	return (*actor)->FindComponentByClass<UTacticalPathfindingController>();
}

void UTacticalPathfindingController::BeginPlay()
{
	Super::BeginPlay();
	graphController = this->GetOwner()->FindComponentByClass<UGraphNodeNetwork>();
	graph = graphController->CreateNetwork();
}

TArray<UPathNode*> UTacticalPathfindingController::RunPathfinding(int startIndex, int endIndex, TArray<TacticalInformation*> tacticalInformations)
{
	if (startIndex >= 0 && startIndex < graph.Num() && endIndex >= 0 && endIndex < graph.Num())
	{
		TArray<const UGraphNode*> visitedNodes = TArray<const UGraphNode*>();
		TArray<UPathNode*> path = UTacticalPathfinder::FindTacticalPath(graph, graph[startIndex], graph[endIndex], new EuclideanDistance(graph[endIndex]), tacticalInformations, visitedNodes);

		return path;
	}
	return TArray<UPathNode*>();
}

float UTacticalPathfindingController::CalculatePathLength(TArray<const UGraphNode*> path)
{
	float pathLength = 0.0f;
	for (int i = 0; i < path.Num() - 1; i++)
	{
		pathLength += path[i]->GetNeighbours()[path[i + 1]];
	}
	return pathLength;
}

void UTacticalPathfindingController::DrawNodes(TArray<UPathNode*> path, FColor color, bool connect)
{
	for (int i = 0; i < path.Num(); i++)
	{
		DrawDebugPoint(GetWorld(), path[i]->node->GetCoordinates(), 10, color, false, 0.0f, 1);
		if (connect && i < path.Num() - 1)
		{
			DrawDebugLine(GetWorld(), path[i]->node->GetCoordinates(), path[i + 1]->node->GetCoordinates(), color, false, 0.0f, 1);
		}
	}
}

void UTacticalPathfindingController::DrawTacticalInformation(TArray<TacticalInformation*> tacticalInformations, FColor color)
{
	TArray<UGraphNode*> nodes = graphController->GetNodes();
	for (int i = 0; i < nodes.Num(); i++)
	{
		float tacticalValue = 0;
		for (int j = 0; j < tacticalInformations.Num(); j++)
		{
			tacticalValue += (tacticalInformations[j]->GetQualityAtIndex(nodes[i]->GetIndex()));
		}
		if (tacticalValue > 0.0f)
		{
			DrawDebugPoint(GetWorld(), nodes[i]->GetCoordinates(), 10, color, false, 0.0f);
		}
	}
}

void UTacticalPathfindingController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

