#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAISettings.h"
#include "InfluenceMapPropagator.h"
#include "GuardAIController.generated.h"

UCLASS()
class TACTICALPATHFINDING_API AGuardAIController : public AAIController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	AActor* actor;
	UGuardAISettings* aiSettings;
};
