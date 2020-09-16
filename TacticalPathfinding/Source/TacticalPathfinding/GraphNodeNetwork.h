#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "GraphNode.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GraphNodeNetwork.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALPATHFINDING_API UGraphNodeNetwork : public USceneComponent
{
	GENERATED_BODY()

public:
	UGraphNodeNetwork();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<UGraphNode*> CreateNetwork();
	TArray<UGraphNode*> GetNodes();
	float GetResolution();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float resolution;

	UPROPERTY(VisibleAnywhere)
		TArray<UGraphNode*> nodes;

	FVector bottomLeft;
	FVector topRight;
	float roomWidth;
	float roomLength;

	void GetRoomSize();
	void GenerateNetwork(const UNavigationSystemV1* navSys);
	void CreateMovementNetwork(const UNavigationSystemV1* navSys);
};
