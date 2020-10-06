#include "GuardAIController.h"

void AGuardAIController::BeginPlay()
{
    Super::BeginPlay();
    actor = this->GetViewTarget();
    aiSettings = actor->FindComponentByClass<UGuardAISettings>();
    Super::MoveToLocation(aiSettings->waypoints[aiSettings->currentWaypointIndex], 20.0f, true, true, true, true);
}

void AGuardAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    aiSettings->currentWaypointIndex++;
    if (aiSettings->currentWaypointIndex >= aiSettings->waypoints.Num())
    {
        aiSettings->currentWaypointIndex = 0;
    }
    Super::MoveToLocation(aiSettings->waypoints[aiSettings->currentWaypointIndex], 20.0f, true, true, true, true);
}