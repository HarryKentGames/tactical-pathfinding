#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TacticalPathfindingController.h"
#include "InfluenceMapPropagator.h"
#include "TacticalPathfindingAgent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALPATHFINDING_API UTacticalPathfindingAgent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTacticalPathfindingAgent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UInfluenceMapPropagator* propagator;
	UTacticalPathfindingController* tacticalPathfindingController;

	TArray<TacticalInformation*> stealthTacticalInformation;

	UPROPERTY(EditAnywhere)
	int destinationIndex;
};
