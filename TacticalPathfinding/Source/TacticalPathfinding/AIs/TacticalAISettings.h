#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfluenceMapPropagator.h"
#include "TacticalAISettings.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALPATHFINDING_API UTacticalAISettings : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTacticalAISettings();
	UPROPERTY(EditAnywhere)
	class AActor* destination;
	UInfluenceMapPropagator* destinationPropagator;

	UPROPERTY(EditAnywhere)
	float pathfinderRefreshInterval;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
