#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "InfluenceMapPropagator.h"

class TacticalInformation
{
protected:
	float weight;
	std::vector<float> tacticalMap;
	UInfluenceMapPropagator* propagator;
public:
	TacticalInformation(float weight, UInfluenceMapPropagator* propagator);
	~TacticalInformation();

	float GetQualityAtIndex(int index);
	virtual void UpdateTacticalMap();
};

class AvoidEnemyTacticalInformation : public TacticalInformation
{
public:
	AvoidEnemyTacticalInformation(float weight, UInfluenceMapPropagator* propagator);
	~AvoidEnemyTacticalInformation();

	void UpdateTacticalMap() override;
};


