#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfluenceMapController.h"
#include <map>
#include "InfluenceMapPropagator.generated.h"

UENUM()
enum Team
{
	NoTeam      UMETA(DisplayName = "NoTeam"),
	Team1     UMETA(DisplayName = "Team1"),
	Team2      UMETA(DisplayName = "Team2"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TACTICALPATHFINDING_API UInfluenceMapPropagator : public UActorComponent
{
	GENERATED_BODY()

public:
	UInfluenceMapPropagator();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UInfluenceMapController* GetInfluenceMapController();
	UGraphNode* GetCurrentNode();
	std::vector<float> GetInfluenceMap();
	void SetInfluenceMap(std::vector<float> influenceMapToSet);
	float GetInfluenceRange();
	std::vector<float> GetViewMap();
	void SetViewMap(std::vector<float> viewMapToSet);
	Team GetTeam();
	TArray<Team> GetAlliedTeams();
	TArray<Team> GetEnemyTeams();

	void UpdatePropagator();
	void PropagateInfluenceMap();

protected:
	virtual void BeginPlay() override;
private:
	UInfluenceMapController* influenceMapController;
	UGraphNode* currentNode;

	UPROPERTY(EditAnywhere)
	float influenceRange;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<Team> team;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<Team>> allies;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<Team>> enemies;

	std::vector<float> influenceMap;
	std::vector<float> viewMap;
	FVector previousLocation;
};
