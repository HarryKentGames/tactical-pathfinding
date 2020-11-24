#include "TacticalPathfinder.h"

UTacticalPathfinder::UTacticalPathfinder()
{
}

UTacticalPathfinder::~UTacticalPathfinder()
{
}

TArray<UPathNode*> UTacticalPathfinder::FindTacticalPath(TArray<UGraphNode*> graph, UGraphNode* start, UGraphNode* end, Heuristic* heuristic, TArray<TacticalInformation*> tacticalInformations, TArray<const UGraphNode*>& visitedNodes)
{
	//Initialise the record for the starting node:
	UTacticalNodeRecord* startRecord = UTacticalNodeRecord::MAKE(start, nullptr, 0.0f, heuristic->Estimate(start));
	//Initialise the necessary data structures:
	UBucketedPriorityQueue<UTacticalNodeRecord>* open = new UBucketedPriorityQueue<UTacticalNodeRecord>();
	UBucketedPriorityQueue<UTacticalNodeRecord>* closed = new UBucketedPriorityQueue<UTacticalNodeRecord>();
	open->AddElement(startRecord, 0);
	UTacticalNodeRecord* currentRecord = NewObject<UTacticalNodeRecord>();
	//Process each node that is added to the open list:
	while (!open->IsEmpty())
	{
		//Get the best estimated node:
		open->GetSmallestElement(&currentRecord);
		//If it is the goal, stop the algorithm:
		if (currentRecord->node == end)
		{
			break;
		}
		//Loop over all of its neighbours:
		for (TPair<UGraphNode*, float> connectedNode : currentRecord->node->GetNeighbours())
		{
			if (connectedNode.Key == currentRecord->node)
			{
				continue;
			}
			//Get the cost estimate for the neighbour:
			float tacticalValue = 0;
			for (int i = 0; i < tacticalInformations.Num(); i++)
			{
				tacticalValue += (tacticalInformations[i]->GetQualityAtIndex(currentRecord->node->GetIndex()) + tacticalInformations[i]->GetQualityAtIndex(connectedNode.Key->GetIndex())) / 2;
			}
			float connectedNodeCost = currentRecord->costSoFar + connectedNode.Value + connectedNode.Value * tacticalValue;
			UTacticalNodeRecord* connectedNodeRecord = NewObject<UTacticalNodeRecord>();
			float connectedNodeHeuristic;
			//If the node has been closed:
			//Skip if the new distance is not less than the current distance
			//remove the entry if it is:
			if (closed->FindByNode(&connectedNodeRecord, connectedNode.Key))
			{
				if (connectedNodeRecord->costSoFar <= connectedNodeCost)
				{
					continue;
				}

				closed->RemoveElement(connectedNodeRecord);

				connectedNodeHeuristic = connectedNodeRecord->estimatedTotalCost - connectedNodeRecord->costSoFar;
			}
			//If the node is open:
			//Skip if the new distance is not less than the current distance
			//Else calculate the new estimated distance:
			else if (open->FindByNode(&connectedNodeRecord, connectedNode.Key))
			{
				if (connectedNodeRecord->costSoFar <= connectedNodeCost)
				{
					continue;
				}

				connectedNodeHeuristic = connectedNodeRecord->estimatedTotalCost - connectedNodeRecord->costSoFar;
			}
			//Else the node is unvisited and needs to be added to the open queue:
			else
			{
				connectedNodeRecord->node = connectedNode.Key;
				connectedNodeHeuristic = heuristic->Estimate(connectedNodeRecord->node);
			}
			float oldEstimatedCost = connectedNodeRecord->estimatedTotalCost;
			//Update the values associated with the record:
			connectedNodeRecord->costSoFar = connectedNodeCost;
			connectedNodeRecord->connectedNode = currentRecord;
			connectedNodeRecord->estimatedTotalCost = connectedNodeCost + connectedNodeHeuristic;
			//Either add to the open list, or update the bucket that the record is in:
			if (!open->ContainsByNode(connectedNode.Key))
			{
				open->AddElement(connectedNodeRecord, connectedNodeCost + connectedNodeHeuristic);
			}
			else
			{
				open->ModifyElement(connectedNodeRecord, oldEstimatedCost, connectedNodeRecord->estimatedTotalCost);
			}
		}
		//Move the current record to the closed list as it has been analysed:
		open->RemoveElement(currentRecord);
		closed->AddElement(currentRecord, currentRecord->estimatedTotalCost);
		visitedNodes.Add(currentRecord->node);
	}

	//If the current record is for the end node, there is a valid path, otherwise there is not:
	if (currentRecord->node != end)
	{
		return TArray<UPathNode*>();
	}
	else
	{
		TArray<UPathNode*> path;
		//Loop over the found path, adding the nodes in reverse order to form the path in the correct direction:
		while (currentRecord->node != start)
		{
			UPathNode* nextNode = UPathNode::MAKE(currentRecord->node, currentRecord->costSoFar - currentRecord->connectedNode->costSoFar);
			path.EmplaceAt(0, nextNode);
			currentRecord = currentRecord->connectedNode;
		}
		UPathNode* startNode = UPathNode::MAKE(currentRecord->node, 0);
		path.EmplaceAt(0, startNode);
		return path;
	}
}