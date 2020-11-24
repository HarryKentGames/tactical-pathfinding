#include "TacticalInformation.h"

TacticalInformation::TacticalInformation(float weight, UInfluenceMapPropagator* propagator)
	: weight(weight), propagator(propagator)
{
}

TacticalInformation::~TacticalInformation()
{
}

float TacticalInformation::GetQualityAtIndex(int index)
{
	return weight * tacticalMap[index];
}

void TacticalInformation::UpdateTacticalMap()
{
}

//===============================================================================================================

AvoidEnemyTacticalInformation::AvoidEnemyTacticalInformation(float weight, UInfluenceMapPropagator* propagator)
	: TacticalInformation(weight, propagator)
{
}

AvoidEnemyTacticalInformation::~AvoidEnemyTacticalInformation()
{
}

void AvoidEnemyTacticalInformation::UpdateTacticalMap()
{
	//Reset the tactical maps:
	tacticalMap = std::vector<float>(propagator->GetInfluenceMapController()->GetNodes().Num());
	std::vector<float> LOSMap = std::vector<float>(propagator->GetInfluenceMapController()->GetNodes().Num());
	//Get the enemy teams influences, and observable areas:
	propagator->GetInfluenceMapController()->GetPropagatorEnemyInfluenceMap(propagator, tacticalMap);
	propagator->GetInfluenceMapController()->GetPropagatorEnemyLOSMap(propagator, LOSMap);
	//Add the two maps together:
	for (int i = 0; i < tacticalMap.size(); i++)
	{
		tacticalMap[i] += LOSMap[i];
	}
}
