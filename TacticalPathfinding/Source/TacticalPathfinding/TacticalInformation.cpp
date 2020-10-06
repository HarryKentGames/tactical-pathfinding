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
	tacticalMap = std::vector<float>(propagator->GetInfluenceMapController()->GetNodes().Num());
	std::vector<float> LOSMap = std::vector<float>(propagator->GetInfluenceMapController()->GetNodes().Num());
	propagator->GetInfluenceMapController()->GetPropagatorEnemyInfluenceMap(propagator, tacticalMap, propagator->GetEnemyTeams());
	propagator->GetInfluenceMapController()->GetPropagatorEnemyLOSMap(propagator, LOSMap, propagator->GetEnemyTeams());
	for (int i = 0; i < tacticalMap.size(); i++)
	{
		tacticalMap[i] += LOSMap[i];
	}
}
