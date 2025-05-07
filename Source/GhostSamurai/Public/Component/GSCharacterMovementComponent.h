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

	virtual void BeginPlay();

public:

	UFUNCTION(BlueprintCallable)
	bool TryAndStartWallRun();

	UFUNCTION(BlueprintCallable)
	void Sprint(bool CheckForWallRun);

	UFUNCTION(BlueprintCallable)
	void StopSprint(bool CheckForWallRun);

	UFUNCTION(BlueprintPure)
	bool IsSprinting() { return bIsSprinting; }

	UFUNCTION(BlueprintPure)
	bool IsWallRunning() { return bIsWallRunning; }

	UFUNCTION(BlueprintCallable)
	void StopWallRun();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement: Sprinting")
	float MaxSprintSpeed = 900.f;

	UPROPERTY(BlueprintReadOnly)
	FHitResult WallRunTrace;

private:

	float MaxWalkSpeedAtStart = 0.f;

	bool bIsSprinting = false;

	bool bIsWallRunning = false;


};
