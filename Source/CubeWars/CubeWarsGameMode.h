// This source code file is protected by the MIT License (MIT)

#pragma once

#include "GameFramework/GameMode.h"

#include "Obstacle.h"
#include "DestroyableObstacle.h"
#include "Pickup.h"
#include "PowerUp.h"

#include "CubeWarsGameMode.generated.h"

/** Possible state of the current match, where a match is all the gameplay that happens on a single map */
namespace MatchState
{
	extern const FName Fight;
}

/**
 * 
 */
UCLASS()
class CUBEWARS_API ACubeWarsGameMode : public AGameMode
{
	GENERATED_BODY()

	ACubeWarsGameMode();
	
	/** Before the login. Prevents that more than two players connect */
	virtual void PreLogin(const FString& Options, const FString& Address, const TSharedPtr<const FUniqueNetId>& UniqueId, FString& ErrorMessage) override;

	/** Starts the match warmup and determines the team of the player */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** Chooses the start of the player based on his team */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/** Match is waiting to start and players are now spawned in */
	virtual void HandleMatchIsWaitingToStart() override;

	/** @return True if ready to Start Match. Games should override this */
	virtual bool ReadyToStartMatch_Implementation() override;

	/** @return true if ready to End Match. Games should override this */
	virtual bool ReadyToEndMatch_Implementation() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartMatch() override;

	virtual void HandleMatchHasEnded() override;

public:
	void playerDied(int32 team);

	virtual bool IsMatchInProgress() const override;

	/** Called by the dying obstacle in order to spawn a new obstacle with its index after some time. */
	void ObstacleDied(int32 ObstacleIndex);

	void requestRematch(int32 team);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	TSubclassOf<AObstacle> DefaultObstacle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	TSubclassOf<ADestroyableObstacle> DefaultDestroyableObstacle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	int32 NumObstacles;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	float ObstacleRespawnTime;

	/** The amount of time the players are allowe to move, but cannot shoot. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	int32 WaitTime;

	void SpawnObstacle(int32 ObstacleIndex);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = PowerUp)
	TArray<TSubclassOf<APowerUp>> PowerUpClasses;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = PowerUp)
	TArray<int32> PowerUpSpawnWeights;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = PowerUp)
	TSubclassOf<APickup> PickupClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = PowerUp)
	float PickupRespawnTime;

	/** The amount of time the players have to wait after the cube explosion. */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = GameplayControl)
	float AfterDeathTime;

	void SpawnPickup();
private:
	float startTimer;
	float pickupRespawnTimer;
	float afterDeathTimer;
	int32 nextSecond;
	int32 winnerTeam;

	TArray<float> ObstacleRespawnArray;
	TArray<ADestroyableObstacle*> ObstacleArray;
	TArray<AActor*> ActorsToRemove;
	bool firstPlayerRematch;
	bool secondPlayerRematch;

	int32 nextGUID;
};
