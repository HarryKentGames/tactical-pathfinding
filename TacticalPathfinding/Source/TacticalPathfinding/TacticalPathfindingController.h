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
	TArray<const UGraphNode*> path;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALPATHFINDING_API UTacticalPathfindingController : public USceneComponent
{
	GENERATED_BODY()

public:
	UTacticalPathfindingController();

	static UTacticalPathfindingController* FindInstanceInWorld(UWorld* world);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RunPathfinding(int startIndex, int endIndex, TArray<TacticalInformation*> tacticalInformations);
	float CalculatePathLength(TArray<const UGraphNode*> path);

	PathfindingDebugInformation* GetDebugInfo();
protected:
	virtual void BeginPlay() override;

private:
	UGraphNodeNetwork* graphController;
	TArray<UGraphNode*> graph;

	PathfindingDebugInformation debugInfo;

	void DrawNodes(TArray<const UGraphNode*> path, FColor color, bool connect);
};
