#include "InfluenceMapController.h"
#include "InfluenceMapPropagator.h"

UInfluenceMapController::UInfluenceMapController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UInfluenceMapController* UInfluenceMapController::FindInstanceInWorld(UWorld* world)
{
	//Get all the actors in the current world:
	TArray<AActor*> actors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(world, AActor::StaticClass(), actors);
	//Find the actor that has an influence map controller attached to it:
	AActor** actor = actors.FindByPredicate([](AActor*& item)
	{
		return item->FindComponentByClass<UInfluenceMapController>() != nullptr;
	});
	//Return the influence map controller component:
	return (*actor)->FindComponentByClass<UInfluenceMapController>();
}

void UInfluenceMapController::BeginPlay()
{
	Super::BeginPlay();

	InitialiseNodeNetwork();
}

void UInfluenceMapController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePropagators();
}

TArray<UGraphNode*> UInfluenceMapController::GetNodes() const
{
	return nodes;
}

UGraphNodeNetwork* UInfluenceMapController::GetNodeNetwork() const
{
	return nodeNetwork;
}

void UInfluenceMapController::AddPropagator(UInfluenceMapPropagator* propagatorToAdd)
{
	propagators.Add(propagatorToAdd);
	propagatorToAdd->SetInfluenceMap(std::vector<float>(nodes.Num()));
	propagatorToAdd->SetViewMap(std::vector<float>(nodes.Num()));
}

void UInfluenceMapController::RemovePropagator(UInfluenceMapPropagator* propagatorToRemove)
{
	propagators.Remove(propagatorToRemove);
}

UGraphNode* UInfluenceMapController::GetClosestNode(FVector coordinates) const
{
	UGraphNode* closestNode = nullptr;
	float minimumDistance = INT_MAX;
	//for each node calculate the distance from the position to the node.
	for (UGraphNode* node : nodes)
	{
		float dist = FVector::Dist(coordinates, node->GetCoordinates());
		//if the distance is smaller than the current minimum distance, replace the closest node.
		if (dist < minimumDistance)
		{
			closestNode = node;
			minimumDistance = dist;
		}
	}
	return closestNode;
}

void UInfluenceMapController::InitialiseNodeNetwork()
{
	nodeNetwork = this->GetOwner()->FindComponentByClass<UGraphNodeNetwork>();
	nodes = nodeNetwork->CreateNetwork();
}

void UInfluenceMapController::UpdatePropagators()
{
	ParallelFor(propagators.Num(), [&](int32 Idx) {
		propagators[Idx]->UpdatePropagator();
	});
}

void UInfluenceMapController::NormaliseInfluenceMap(std::vector<float>& influenceMap)
{
	//Calculate the maximum influence value in the map, taking into account negatives:
	float maxValue = *std::max_element(influenceMap.begin(), influenceMap.end());
	float minValue = *std::min_element(influenceMap.begin(), influenceMap.end());
	maxValue = abs(minValue) > maxValue ? abs(minValue) : maxValue;
	//Normalise all values in the map based on this maximum value:
	for (int i = 0; i < influenceMap.size(); i++)
	{
		if (influenceMap[i] != 0)
		{
			influenceMap[i] /= maxValue;
		}
	}
}

void UInfluenceMapController::GetPropagatorInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	influenceMap = propagator->GetInfluenceMap();
}

void UInfluenceMapController::GetPropagatorTeamInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() == p->GetTeam())
		{
			std::vector<float> pInfluenceMap = p->GetInfluenceMap();
			for (int i = 0; i < influenceMap.size(); i++)
			{
				influenceMap[i] += pInfluenceMap[i];
			}
		}
	}
}

void UInfluenceMapController::GetPropagatorAllyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() == p->GetTeam() || propagator->GetAlliedTeams().Contains(p->GetTeam()))
		{
			std::vector<float> pInfluenceMap = p->GetInfluenceMap();
			for (int i = 0; i < influenceMap.size(); i++)
			{
				influenceMap[i] += pInfluenceMap[i];
			}
		}
	}
}

void UInfluenceMapController::GetPropagatorEnemyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() != p->GetTeam() && propagator->GetEnemyTeams().Contains(p->GetTeam()))
		{
			std::vector<float> pInfluenceMap = p->GetInfluenceMap();
			for (int i = 0; i < influenceMap.size(); i++)
			{
				influenceMap[i] += pInfluenceMap[i];
			}
		}
	}
}

void UInfluenceMapController::GetCompleteInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> allyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorAllyInfluenceMap(propagator, allyInfluenceMap);
	std::vector<float> enemyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorEnemyInfluenceMap(propagator, enemyInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = allyInfluenceMap[i] - enemyInfluenceMap[i];
	}
}

void UInfluenceMapController::GetTensionMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> allyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorAllyInfluenceMap(propagator, allyInfluenceMap);
	std::vector<float> enemyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorEnemyInfluenceMap(propagator, enemyInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = allyInfluenceMap[i] * enemyInfluenceMap[i];
	}
}

void UInfluenceMapController::GetVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> tensionMap = std::vector<float>(influenceMap.size());
	GetTensionMap(propagator, tensionMap);
	std::vector<float> completeInfluenceMap = std::vector<float>(influenceMap.size());
	GetCompleteInfluenceMap(propagator, completeInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = tensionMap[i] - abs(completeInfluenceMap[i]);
	}
}

void UInfluenceMapController::GetDirectedVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> tensionMap = std::vector<float>(influenceMap.size());
	GetTensionMap(propagator, tensionMap);
	std::vector<float> completeInfluenceMap = std::vector<float>(influenceMap.size());
	GetCompleteInfluenceMap(propagator, completeInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = tensionMap[i] * completeInfluenceMap[i];
	}
}

void UInfluenceMapController::GetPropagatorLOSMap(UInfluenceMapPropagator* propagator, std::vector<float>& LOSMap)
{
	LOSMap = propagator->GetViewMap();
}

void UInfluenceMapController::GetPropagatorAllyLOSMap(UInfluenceMapPropagator* propagator, std::vector<float>& LOSMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() == p->GetTeam() || propagator->GetAlliedTeams().Contains(p->GetTeam()))
		{
			std::vector<float> pLOSMap = p->GetViewMap();
			for (int i = 0; i < LOSMap.size(); i++)
			{
				LOSMap[i] += pLOSMap[i];
			}
		}
	}
}

void UInfluenceMapController::GetPropagatorEnemyLOSMap(UInfluenceMapPropagator* propagator, std::vector<float>& LOSMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() != p->GetTeam() && propagator->GetEnemyTeams().Contains(p->GetTeam()))
		{
			std::vector<float> pLOSMap = p->GetViewMap();
			for (int i = 0; i < LOSMap.size(); i++)
			{
				LOSMap[i] += pLOSMap[i];
			}
		}
	}
}


