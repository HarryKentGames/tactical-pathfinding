#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "InfluenceMapPropagator.h"
#include "Navigation/MetaNavMeshPath.h"
#include "TacticalAISettings.h"
#include "TacticalPathfindingController.h"
#include "TacticalAIController.generated.h"

UCLASS()
class TACTICALPATHFINDING_API ATacticalAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	AActor* actor;
	UTacticalAISettings* aiSettings;
	UInfluenceMapPropagator* propagator;
	UTacticalPathfindingController* tacticalPathfindingController;

	TArray<TacticalInformation*> stealthTacticalInformation;
	TArray<UPathNode*> path;
	float timeSinceLastPathfinderRefresh;
};
