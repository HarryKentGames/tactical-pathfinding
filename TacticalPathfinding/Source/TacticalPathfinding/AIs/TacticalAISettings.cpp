#include "TacticalAISettings.h"

UTacticalAISettings::UTacticalAISettings()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTacticalAISettings::BeginPlay()
{
	Super::BeginPlay();
	//Get the destination's propagator:
	if (destination != nullptr)
	{
		destinationPropagator = destination->FindComponentByClass<UInfluenceMapPropagator>();
	}
}

void UTacticalAISettings::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

