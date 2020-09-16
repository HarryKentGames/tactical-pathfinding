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
	int GetIndex();
	void AddNeighbour(UGraphNode* neighbour, float distance);
	TMap<UGraphNode*, float> GetNeighbours() const;

private:
	UPROPERTY()
	FVector coordinates;
	int index;
	UPROPERTY()
	TMap<UGraphNode*, float> neighbours;
};