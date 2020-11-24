#include "TacticalNodeRecord.h"

UTacticalNodeRecord::UTacticalNodeRecord()
{
};

UTacticalNodeRecord* UTacticalNodeRecord::MAKE(UGraphNode* node, UTacticalNodeRecord* connectedNode, float costSoFar, float estimatedTotalCost)
{
	UTacticalNodeRecord* record = NewObject<UTacticalNodeRecord>();
	record->Init(node, connectedNode, costSoFar, estimatedTotalCost);
	return record;
};

void UTacticalNodeRecord::Init(UGraphNode* nodeValue, UTacticalNodeRecord* connectedNodeValue, float costSoFarValue, float estimatedTotalCostValue)
{
	node = nodeValue;
	connectedNode = connectedNodeValue;
	costSoFar = costSoFarValue;
	estimatedTotalCost = estimatedTotalCostValue;
};