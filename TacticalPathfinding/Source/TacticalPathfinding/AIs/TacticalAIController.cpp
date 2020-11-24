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
    if (propagator->GetCurrentNode() != nullptr && timeSinceLastPathfinderRefresh >= aiSettings->pathfinderRefreshInterval)
    {
        timeSinceLastPathfinderRefresh = 0;
        path = tacticalPathfindingController->RunPathfinding(propagator->GetCurrentNode()->GetIndex(), aiSettings->destinationPropagator->GetCurrentNode()->GetIndex(), stealthTacticalInformation);
        
        UPathFollowingComponent* pathFollowingComponent = GetPathFollowingComponent();
        
        TArray<FVector> locations;
        for (UPathNode* pathNode : path)
        {
            locations.Add(pathNode->node->GetCoordinates());
        }

        AController* controller = Cast<AController>(this);
        FMetaNavMeshPath* MetaNavMeshPath = new FMetaNavMeshPath(locations, *controller);

        TSharedPtr<FMetaNavMeshPath, ESPMode::ThreadSafe> MetaPathPtr(MetaNavMeshPath);
        pathFollowingComponent->RequestMove(FAIMoveRequest(), MetaPathPtr);        
    }
}