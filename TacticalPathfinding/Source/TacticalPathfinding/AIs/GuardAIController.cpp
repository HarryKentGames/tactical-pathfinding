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
    //Increment waypoint, loop around to 0 if goes beyond score of waypoint list:
    aiSettings->currentWaypointIndex = ++(aiSettings->currentWaypointIndex) < aiSettings->waypoints.Num() ? aiSettings->currentWaypointIndex : 0;
    //Move to new Waypoint:
    Super::MoveToLocation(aiSettings->waypoints[aiSettings->currentWaypointIndex], 20.0f, true, true, true, true);
}