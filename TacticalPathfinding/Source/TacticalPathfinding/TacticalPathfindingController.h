#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include <chrono>
#include "GraphNodeNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "TacticalPathfinder.h"
#include "TacticalPathfindingController.generated.h"

struct PathfindingDebugInformation
{
	float timeTaken;
	TArray<const UGraphNode*> visitedNodes;
	TArray<UPathNode*> path;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALPATHFINDING_API UTacticalPathfindingController : public USceneComponent
{
	GENERATED_BODY()

public:
	UTacticalPathfindingController();

	static UTacticalPathfindingController* FindInstanceInWorld(UWorld* world);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<UPathNode*> RunPathfinding(int startIndex, int endIndex, TArray<TacticalInformation*> tacticalInformations);
	float CalculatePathLength(TArray<const UGraphNode*> path);

	PathfindingDebugInformation* GetDebugInfo();
	PathfindingDebugInformation debugInfo;
protected:
	virtual void BeginPlay() override;

private:
	UGraphNodeNetwork* graphController;
	TArray<UGraphNode*> graph;

	void DrawNodes(TArray<UPathNode*> path, FColor color, bool connect);
};
