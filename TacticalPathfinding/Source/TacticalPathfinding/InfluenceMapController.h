#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GraphNodeNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "TimerManager.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "InfluenceMapController.generated.h"

class UInfluenceMapPropagator;
enum Team;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALPATHFINDING_API UInfluenceMapController : public USceneComponent
{
	GENERATED_BODY()

public:

	UInfluenceMapController();

	static UInfluenceMapController* FindInstanceInWorld(UWorld* world);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Getters and Setters:
	UGraphNodeNetwork* GetNodeNetwork() const;
	TArray<UGraphNode*> GetNodes() const;
	void AddPropagator(UInfluenceMapPropagator* propagatorToAdd);
	void RemovePropagator(UInfluenceMapPropagator* propagatorToRemove);
	UGraphNode* GetClosestNode(FVector coordinates) const;

	void NormaliseInfluenceMap(std::vector<float>& influenceMap);
	void GetPropagatorInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetPropagatorTeamInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetPropagatorAllyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap, TArray<Team> teamMask);
	void GetPropagatorEnemyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap, TArray<Team> teamMask);
	void GetCompleteInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap, TArray<Team> alliedTeamMask, TArray<Team> enemyTeamMask);
	void GetTensionMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap, TArray<Team> alliedTeamMask, TArray<Team> enemyTeamMask);
	void GetVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap, TArray<Team> alliedTeamMask, TArray<Team> enemyTeamMask);
	void GetDirectedVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap, TArray<Team> alliedTeamMask, TArray<Team> enemyTeamMask);

	void GetPropagatorLOSMap(UInfluenceMapPropagator* propagator, std::vector<float>& LOSMap);
	void GetPropagatorAllyLOSMap(UInfluenceMapPropagator* propagator, std::vector<float>& LOSMap, TArray<Team> teamMask);
	void GetPropagatorEnemyLOSMap(UInfluenceMapPropagator* propagator, std::vector<float>& LOSMap, TArray<Team> teamMask);

	void DebugDraw();

protected:
	virtual void BeginPlay() override;

private:
	UGraphNodeNetwork* nodeNetwork;
	UPROPERTY()
	TArray<UGraphNode*> nodes;
	UPROPERTY()
	TArray<UInfluenceMapPropagator*> propagators;

	void InitialiseNodeNetwork();
	void UpdatePropagators();
};
