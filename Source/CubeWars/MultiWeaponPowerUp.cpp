// This source code file is protected by the MIT License (MIT)

#include "CubeWars.h"
#include "MultiWeaponPowerUp.h"

AMultiWeaponPowerUp::AMultiWeaponPowerUp() : MaxProjectileCount(8), MinProjectileCount(5), MaxPitchAngleDerivation(5.0f), MaxYawAngleDerivation(5.0f)
{

}

bool AMultiWeaponPowerUp::OnShoot_Implementation()
{
	// try and fire a projectile
	if(ProjectileClass != nullptr)
	{
		int32 ProjectileCount;
		FRandomStream RandStream;
		RandStream.GenerateNewSeed();

		if(MaxProjectileCount == MinProjectileCount)
		{
			ProjectileCount = MaxProjectileCount;
		}
		else
		{
			ProjectileCount = RandStream.RandRange(MinProjectileCount, MaxProjectileCount);
		}

		for(int32 proj = 0; proj < ProjectileCount; ++proj)
		{
			float PitchDerivation = 0.0f;
			float YawDerivation = 0.0f;

			if(MaxPitchAngleDerivation > 0)
			{
				PitchDerivation = RandStream.FRandRange(-MaxPitchAngleDerivation, MaxPitchAngleDerivation);
			}

			if(MaxYawAngleDerivation > 0)
			{
				YawDerivation = RandStream.FRandRange(-MaxYawAngleDerivation, MaxYawAngleDerivation);
			}

			const FRotator SpawnRotation = GetPlayerCube()->GetActorRotation() + FRotator(PitchDerivation, YawDerivation, 0.0f);
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector OffsetVector(120.0f, 0.0f, 0.0f);
			const FVector SpawnLocation = GetPlayerCube()->GetActorLocation() + SpawnRotation.RotateVector(OffsetVector);

			UWorld* const World = GetWorld();
			if(World != nullptr)
			{
				// spawn the projectile at the muzzle
				AProjectile* projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);

				projectile->SetInstigator(GetPlayerCube()->GetController());
			}
		}

		return true;
	}

	return false;
}

