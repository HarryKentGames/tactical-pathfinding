#pragma once

#include "CoreMinimal.h"
#include "GraphNode.h"
#include "UObject/NoExportTypes.h"
#include "TacticalNodeRecord.generated.h"

UCLASS()
class TACTICALPATHFINDING_API UTacticalNodeRecord : public UObject
{
	GENERATED_BODY()
public:
	UGraphNode* node;
	UTacticalNodeRecord* connectedNode;
	float costSoFar;
	float estimatedTotalCost;

	UTacticalNodeRecord();

	static UTacticalNodeRecord* MAKE(UGraphNode* node, UTacticalNodeRecord* connectedNode, float costSoFar, float estimatedTotalCost);
	void Init(UGraphNode* nodeValue, UTacticalNodeRecord* connectedNodeValue, float costSoFarValue, float estimatedTotalCostValue);
};
