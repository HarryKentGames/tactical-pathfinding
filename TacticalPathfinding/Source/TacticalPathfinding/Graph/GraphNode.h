#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GraphNode.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALPATHFINDING_API UGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UGraphNode();
	~UGraphNode();
	static UGraphNode* MAKE(FVector coordinates, int index);
	void Init(FVector coordinateValue, int indexValue);

	//Getters and Setters:
	void SetCoordinates(FVector coordinates);
	FVector GetCoordinates() const;
	int GetIndex() const;
	void AddNeighbour(UGraphNode* neighbour, float distance);
	TMap<UGraphNode*, float> GetNeighbours() const;
	void AddInViewNode(UGraphNode* node, float distance);
	TMap<UGraphNode*, float> GetInViewNodes() const;
	void AddOutOfViewNode(UGraphNode* node);
	TSet<UGraphNode*> GetOutOfViewNodes() const;


private:
	UPROPERTY()
	FVector coordinates;
	int index;
	UPROPERTY(VisibleAnywhere)
	TMap<UGraphNode*, float> neighbours;
	UPROPERTY(VisibleAnywhere)
	TMap<UGraphNode*, float> inViewNodes;
	UPROPERTY(VisibleAnywhere)
	TSet<UGraphNode*> outOfViewNodes;
};
