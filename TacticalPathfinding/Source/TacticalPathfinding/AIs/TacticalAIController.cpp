#include "TacticalAIController.h"

void ATacticalAIController::BeginPlay()
{
    Super::BeginPlay();
    actor = this->GetViewTarget();
    propagator = actor->FindComponentByClass<UInfluenceMapPropagator>();
    aiSettings = actor->FindComponentByClass<UTacticalAISettings>();
    tacticalPathfindingController = UTacticalPathfindingController::FindInstanceInWorld(GetWorld());
    stealthTacticalInformation.Add(new AvoidEnemyTacticalInformation(5.0f, propagator));
}

void ATacticalAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //Update agent's tactical information:
    for (TacticalInformation* tacticalInformation : stealthTacticalInformation)
    {
        tacticalInformation->UpdateTacticalMap();
    }
    timeSinceLastPathfinderRefresh += DeltaTime;
    //Only refresh the path every X amount of time:
    if (propagator->GetCurrentNode() != nullptr && timeSinceLastPathfinderRefresh >= aiSettings->pathfinderRefreshInterval)
    {
        timeSinceLastPathfinderRefresh = 0;
        //Find the new path:
        path = tacticalPathfindingController->RunPathfinding(propagator->GetCurrentNode()->GetIndex(), aiSettings->destinationPropagator->GetCurrentNode()->GetIndex(), stealthTacticalInformation);
        //Create a new path for the agent to follow:
        UPathFollowingComponent* pathFollowingComponent = GetPathFollowingComponent();
        //Add all the waypoints from the calculated path:
        TArray<FVector> locations;
        for (UPathNode* pathNode : path)
        {
            locations.Add(pathNode->node->GetCoordinates());
        }
        //Assign the new path to the agent:
        AController* controller = Cast<AController>(this);
        FMetaNavMeshPath* MetaNavMeshPath = new FMetaNavMeshPath(locations, *controller);
        TSharedPtr<FMetaNavMeshPath, ESPMode::ThreadSafe> MetaPathPtr(MetaNavMeshPath);
        pathFollowingComponent->RequestMove(FAIMoveRequest(), MetaPathPtr);        
    }
    tacticalPathfindingController->DrawNodes(path, FColor::Red, true);
}