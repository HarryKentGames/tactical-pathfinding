#include "InfluenceMapPropagator.h"

UInfluenceMapPropagator::UInfluenceMapPropagator()
{
	PrimaryComponentTick.bCanEverTick = true;
	previousLocation = FVector(INT_MAX, INT_MAX, INT_MAX);
}

void UInfluenceMapPropagator::BeginPlay()
{
	influenceMapController = UInfluenceMapController::FindInstanceInWorld(GetWorld());
	influenceMapController->AddPropagator(this);
}

void UInfluenceMapPropagator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

UInfluenceMapController* UInfluenceMapPropagator::GetInfluenceMapController()
{
	return influenceMapController;
}

UGraphNode* UInfluenceMapPropagator::GetCurrentNode()
{
	return currentNode;
}

std::vector<float> UInfluenceMapPropagator::GetInfluenceMap()
{
	return influenceMap;
}

void UInfluenceMapPropagator::SetInfluenceMap(std::vector<float> influenceMapToSet)
{
	influenceMap = influenceMapToSet;
}

std::vector<float> UInfluenceMapPropagator::GetViewMap()
{
	return viewMap;
}

void UInfluenceMapPropagator::SetViewMap(std::vector<float> viewMapToSet)
{
	viewMap = viewMapToSet;
}

void UInfluenceMapPropagator::UpdatePropagator()
{
	//Get the location at the feet of the propagator:
	FVector boundsOrigin = FVector();
	FVector boundsExtents = FVector();
	this->GetOwner()->GetActorBounds(true, boundsOrigin, boundsExtents);
	FVector actorLocation = boundsOrigin;
	actorLocation.Z = actorLocation.Z - boundsExtents.Z;
	if (actorLocation != previousLocation)
	{
		previousLocation = actorLocation;
		//Get the closest node to this location, and if it is different to the current node, update the propagators position:
		UGraphNode* newNode = influenceMapController->GetClosestNode(actorLocation);
		if (newNode != currentNode)
		{
			if (currentNode != nullptr)
			{
				influenceMap[currentNode->GetIndex()] = 0.0f;
			}
			currentNode = newNode;
			influenceMap[currentNode->GetIndex()] = 1.0f;
			PropagateInfluenceMap();
		}
	}
}

void UInfluenceMapPropagator::PropagateInfluenceMap()
{
	std::map<UGraphNode*, float> unvisitedNodes = std::map<UGraphNode*, float>();
	std::map<UGraphNode*, float> visitedNodes = std::map<UGraphNode*, float>();
	std::vector<float> influenceMapBuffer = std::vector<float>(influenceMap.size());

	unvisitedNodes[currentNode] = 0.0f;

	while (unvisitedNodes.size() > 0)
	{
		std::map<UGraphNode*, float>::iterator firstElement = unvisitedNodes.begin();
		UGraphNode* node = firstElement->first;
		float distance = firstElement->second;
		influenceMapBuffer[node->GetIndex()] = 1.0f - (1.0f * (distance / influenceRange));
		visitedNodes[node] = distance;
		for (TPair<UGraphNode*, float> neighbour : node->GetNeighbours())
		{
			//Check that the neighbour is in influence range:
			float neighbourDistance = distance + neighbour.Value;
			if (neighbourDistance > influenceRange)
			{
				continue;
			}
			//Check that the neighbour has not already been visited, with a shorter distance:
			bool inVisited = visitedNodes.find(neighbour.Key) != visitedNodes.end();
			if (inVisited && visitedNodes[neighbour.Key] <= neighbourDistance)
			{
				continue;
			}
			//Check that the neighbour is not currently known about with a shorter distance:
			bool inUnvisited = unvisitedNodes.find(neighbour.Key) != unvisitedNodes.end();
			if (inUnvisited && unvisitedNodes[neighbour.Key] <= neighbourDistance)
			{
				continue;
			}
			//If all checks pass, add the node to the list of unvisited nodes:
			unvisitedNodes[neighbour.Key] = neighbourDistance;
		}
		unvisitedNodes.erase(node);
	}
	//Update the influence map:
	SetInfluenceMap(influenceMapBuffer);

	std::vector<float> newViewMap = std::vector<float>(influenceMap.size());
	TArray<UGraphNode*> inViewNodes;
	currentNode->GetInViewNodes().GenerateKeyArray(inViewNodes);
	for (UGraphNode* inViewNode : inViewNodes)
	{
		newViewMap[inViewNode->GetIndex()] = 1.0f;
	}
	SetViewMap(newViewMap);
}

float UInfluenceMapPropagator::GetInfluenceRange()
{
	return influenceRange;
}

Team UInfluenceMapPropagator::GetTeam()
{
	return team;
}

TArray<Team> UInfluenceMapPropagator::GetAlliedTeams()
{
	TArray<Team> allyList;
	for (int i = 0; i < allies.Num(); i++)
	{
		allyList.Push(allies[i].GetValue());
	}
	return allyList;
}

TArray<Team> UInfluenceMapPropagator::GetEnemyTeams()
{
	TArray<Team> enemyList;
	for (int i = 0; i < enemies.Num(); i++)
	{
		enemyList.Push(enemies[i].GetValue());
	}
	return enemyList;
}
