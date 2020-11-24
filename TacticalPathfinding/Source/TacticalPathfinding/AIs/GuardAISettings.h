#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GuardAISettings.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICALPATHFINDING_API UGuardAISettings : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGuardAISettings();
	UPROPERTY(EditAnywhere)
	TArray<FVector> waypoints;
	int currentWaypointIndex;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
};
