#include "GraphNode.h"

UGraphNode::UGraphNode()
	: coordinates(FVector()), index(0)
{
};

UGraphNode::~UGraphNode()
{
}

UGraphNode* UGraphNode::MAKE(FVector coordinates, int index)
{
	UGraphNode* node = NewObject<UGraphNode>();
	node->Init(coordinates, index);
	return node;
}


void UGraphNode::Init(FVector coordinateValue, int indexValue)
{
	coordinates = coordinateValue;
	index = indexValue;
};

void UGraphNode::SetCoordinates(FVector coordinateValue)
{
	coordinates = coordinateValue;
}

FVector UGraphNode::GetCoordinates() const
{
	return coordinates;
}

int UGraphNode::GetIndex() const
{
	return index;
}

void UGraphNode::AddNeighbour(UGraphNode* neighbour, float distance)
{
	neighbours.Add(neighbour, distance);
}

TMap<UGraphNode*, float> UGraphNode::GetNeighbours() const
{
	return neighbours;
}

void UGraphNode::AddInViewNode(UGraphNode* node, float distance)
{
	inViewNodes.Add(node, distance);
}

TMap<UGraphNode*, float> UGraphNode::GetInViewNodes() const
{
	return inViewNodes;
}

void UGraphNode::AddOutOfViewNode(UGraphNode* node)
{
	outOfViewNodes.Add(node);
}

TSet<UGraphNode*> UGraphNode::GetOutOfViewNodes() const
{
	return outOfViewNodes;
}
