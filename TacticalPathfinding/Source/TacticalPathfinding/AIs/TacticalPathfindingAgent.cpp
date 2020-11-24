#include "TacticalPathfindingAgent.h"

UTacticalPathfindingAgent::UTacticalPathfindingAgent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTacticalPathfindingAgent::BeginPlay()
{
	Super::BeginPlay();
	propagator = this->GetOwner()->FindComponentByClass<UInfluenceMapPropagator>();
	tacticalPathfindingController = UTacticalPathfindingController::FindInstanceInWorld(GetWorld());
	stealthTacticalInformation.Add(new AvoidEnemyTacticalInformation(5.0f, propagator));
}

void UTacticalPathfindingAgent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (TacticalInformation* tacticalInformation : stealthTacticalInformation)
	{
		tacticalInformation->UpdateTacticalMap();
	}
	tacticalPathfindingController->RunPathfinding(propagator->GetCurrentNode()->GetIndex(), destinationIndex, stealthTacticalInformation);
}

