// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GSCharacterMovementComponent.generated.h"

UENUM()
enum ECustomMovementModes
{
	WALLRUN
};

/**
 * 
 */
UCLASS()
class GHOSTSAMURAI_API UGSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:

	void PhysWallRun(float deltaTime, int32 Iterations);

protected:

	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const;
	
	virtual void PhysCustom(float deltaTime, int32 Iterations);

	virtual float GetMaxAcceleration() const;

	virtual void BeginPlay();

public:

	UFUNCTION(BlueprintCallable)
	bool TryAndStartWallRun();

	UFUNCTION(BlueprintCallable)
	void StartSprint(bool CheckForWallRun);

	UFUNCTION(BlueprintCallable)
	void StopSprint(bool CheckForWallRun);

	UFUNCTION(BlueprintPure)
	bool IsSprinting() { return bIsSprinting; }

	UFUNCTION(BlueprintPure)
	bool IsWallRunning() const { return bIsWallRunning; }

	UFUNCTION(BlueprintCallable)
	void StopWallRun();

	UFUNCTION(BlueprintPure)
	float GetCharacterWallRunDirection() { return CharacterWallRunDirection; }


public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Sprinting")
	float MaxSprintSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Wall Run")
	float WallRunDetectionDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Wall Run")
	float WallRunMaintainDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Wall Run")
	float MaxWallRunAcceleration = 2048.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Wall Run")
	float MaxWallRunSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Wall Run")
	float WallRunFriction = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Wall Run")
	float BrakingDecelerationWallRun = 2048.f;

	UPROPERTY(BlueprintReadOnly)
	FHitResult WallRunTrace;

	UPROPERTY(BlueprintReadOnly)
	FRotator TowardsWallRotation;

private:

	bool bPreventWallRun = false;

	float MaxWalkSpeedAtStart = 0.f;

	bool bIsSprinting = false;

	bool bIsWallRunning = false;

	float CharacterWallRunDirection = 0.f;

	void PerformWallRunEndJump();

	void CantWallRunFor(float seconds);

	UFUNCTION()
	void StopPreventWallRun();

	FTimerHandle PreventWallRunTimerHandle;

};
