#include "GraphNodeNetwork.h"

UGraphNodeNetwork::UGraphNodeNetwork()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGraphNodeNetwork::BeginPlay()
{
	Super::BeginPlay();
	CreateNetwork();
}

void UGraphNodeNetwork::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<UGraphNode*> UGraphNodeNetwork::GetNodes()
{
	return nodes;
}

float UGraphNodeNetwork::GetResolution()
{
	return resolution;
}

TArray<UGraphNode*> UGraphNodeNetwork::CreateNetwork()
{
	const UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());

	GetRoomSize();
	GenerateNetwork(navSys);
	CreateMovementNetwork(navSys);
	CreateViewNetwork();
	return nodes;
}

void UGraphNodeNetwork::GetRoomSize()
{
	//Get all the children objects:
	TArray<AActor*> children = TArray<AActor*>();
	this->GetOwner()->GetAttachedActors(children);
	//Loop over all children and check if their bounds are out of the box created by the corner markers:
	for (AActor* child : children)
	{
		FVector childOrigin = FVector();
		FVector childExtents = FVector();
		child->GetActorBounds(false, childOrigin, childExtents);
		FVector childBottomLeft = (childOrigin - childExtents);
		FVector childTopRight = (childOrigin + childExtents);
		//If the bounds of the child would expand the box, replace the respective corner:
		if (childBottomLeft.X <= bottomLeft.X && childBottomLeft.Y <= bottomLeft.Y)
		{
			bottomLeft = childBottomLeft;
		}
		else if (childTopRight.X >= topRight.X && childTopRight.Y >= topRight.Y)
		{
			topRight = childTopRight;
		}
	}
	//Calculate the size of the room:
	roomWidth = ceil(topRight.X - bottomLeft.X);
	roomLength = ceil(topRight.Y - bottomLeft.Y);
}

void UGraphNodeNetwork::GenerateNetwork(const UNavigationSystemV1* navSys)
{
	float maxY = navSys->GetWorldBounds().Max.Y;
	//Loop over the room:
	for (float x = bottomLeft.X; x <= topRight.X; x += resolution)
	{
		for (float y = bottomLeft.Y; y <= topRight.Y; y += resolution)
		{
			//Cast a ray through the level, and record all the hit points:
			TArray<FHitResult> hitResults;
			FVector start = FVector(x, y, this->GetOwner()->GetActorLocation().Z + maxY);
			FVector end = start + FVector::DownVector * maxY;
			FCollisionQueryParams collisionParams;
			FCollisionResponseParams collisionResponseParams = FCollisionResponseParams(ECollisionResponse::ECR_Overlap);
			GetWorld()->LineTraceMultiByChannel(hitResults, start, end, ECC_WorldDynamic, collisionParams, collisionResponseParams);
			//Loop over all the hit points:
			for (FHitResult hitResult : hitResults)
			{
				//Get position on NavMesh:
				FVector point = hitResult.ImpactPoint;
				FNavLocation navLocation = FNavLocation();
				FVector queryExtents = FVector(100.0f, 100.0f, 100.0f);
				navSys->ProjectPointToNavigation(point, navLocation, queryExtents);
				//If position on NavMesh is same as expected position add the node to the network:
				if (navLocation.Location.X == point.X && navLocation.Location.Y == point.Y)
				{
					UGraphNode** existingNode = nodes.FindByPredicate([navLocation](UGraphNode*& item)
					{
						return item->GetCoordinates() == navLocation;
					});
					if (!existingNode)
					{
						nodes.Add(UGraphNode::MAKE(navLocation.Location, nodes.Num()));
					}
				}
			}
		}
	}
}

void UGraphNodeNetwork::CreateMovementNetwork(const UNavigationSystemV1* navSys)
{
	//Loop over all the nodes:
	for (UGraphNode* node : nodes)
	{
		//Loop over expected neighbour positions:
		FVector position = node->GetCoordinates();
		for (float x = position.X - resolution; x <= position.X + resolution; x += resolution)
		{
			for (float y = position.Y - resolution; y <= position.Y + resolution; y += resolution)
			{
				//Skip itself:
				if (x == position.X && y == position.Y)
				{
					continue;
				}
				FVector neighbourPosition = FVector(x, y, position.Z);
				//Get the neighbours point on the navmesh:
				FNavLocation navLocation = FNavLocation();
				FVector queryExtents = FVector(100.0f, 100.0f, 100.0f);
				navSys->ProjectPointToNavigation(neighbourPosition, navLocation, queryExtents);
				//If the location on the nav mesh lines up with the expected location:
				UGraphNode** neighbourNode = nodes.FindByPredicate([navLocation](UGraphNode*& item)
				{
					return item->GetCoordinates() == navLocation;
				});
				if (neighbourNode)
				{
					//Get the length of path between the two points:
					UNavigationPath* path = navSys->FindPathToLocationSynchronously(GetWorld(), position, (*neighbourNode)->GetCoordinates());
					if (!path->IsPartial())
					{
						float pathLength = path->GetPathLength();
						//If the length between the nodes is less than the actual distance, link the nodes, if they arent already:
						if (pathLength <= FVector::Dist((*neighbourNode)->GetCoordinates(), position))
						{
							if (!node->GetNeighbours().Contains(*neighbourNode))
							{
								(*neighbourNode)->AddNeighbour(node, pathLength);
								node->AddNeighbour(*neighbourNode, pathLength);
							}
						}
					}
				}
			}
		}
	}
}

void UGraphNodeNetwork::CreateViewNetwork()
{
	//Loop over all nodes:
	for (UGraphNode* node : nodes)
	{
		//Loop over all other nodes:
		for (UGraphNode* otherNode : nodes)
		{
			//Ignore pairs that have already been compared:
			if (node == otherNode || node->GetInViewNodes().Contains(otherNode) || node->GetOutOfViewNodes().Contains(otherNode))
			{
				continue;
			}
			//Raycast between the nodes:
			FHitResult hitResult = FHitResult();
			FCollisionQueryParams collisionParams;
			FCollisionResponseParams collisionResponseParams = FCollisionResponseParams(ECollisionResponse::ECR_Overlap);
			//If the raycast is not blocked, nodes are in view of each other:
			if (!GetWorld()->LineTraceSingleByChannel(hitResult, node->GetCoordinates(), otherNode->GetCoordinates(), ECC_WorldDynamic, ECollisionChannel::ECC_WorldStatic))
			{
				node->AddInViewNode(otherNode, hitResult.Distance);
				otherNode->AddInViewNode(node, hitResult.Distance);
			}
			//Else if the raycast is blocked, nodes are not in view of eachother:
			else
			{
				node->AddOutOfViewNode(otherNode);
				otherNode->AddOutOfViewNode(node);
			}
		}
	}
}

