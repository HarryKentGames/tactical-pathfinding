#include "GuardAISettings.h"

UGuardAISettings::UGuardAISettings()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGuardAISettings::BeginPlay()
{
	Super::BeginPlay();	
}

void UGuardAISettings::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

