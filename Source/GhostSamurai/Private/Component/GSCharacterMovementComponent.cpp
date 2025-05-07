// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GSCharacterMovementComponent.h"
#include "Actors/BaseWallRunObject.h"
#include "GameFramework/Character.h"

void UGSCharacterMovementComponent::PhysWallRun(float deltaTime, int32 Iterations)
{

	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	if (!CharacterOwner || (!GetCharacterOwner()->Controller && !bRunPhysicsWithNoController  && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}

	if (!UpdatedComponent->IsQueryCollisionEnabled())
	{
		SetMovementMode(MOVE_Walking);
		return;
	}

	float remainingTime = deltaTime;

	const EMovementMode StartingMovementMode = MovementMode;
	const uint8 StartingCustomMovementMode = CustomMovementMode;

	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		const FVector OldLocation = UpdatedComponent->GetComponentLocation();


		const FVector OldVelocity = Velocity;

		CalcVelocity(timeTick, GroundFriction, false, BrakingDecelerationWalking);



		// Compute move parameters
		FVector ActorMoveVelocity = Velocity;

		FVector WallBackwardVector = WallRunTrace.Normal;
		FVector approxWallLeftVector = FVector::CrossProduct(WallBackwardVector, GetOwner()->GetActorUpVector());
		FVector WallUpVector = FVector::CrossProduct(approxWallLeftVector, WallBackwardVector);
		FVector WallLeftVector = FVector::CrossProduct(WallUpVector, WallBackwardVector);

		float WallForwardMagnitude = Velocity.ProjectOnTo(-WallBackwardVector).Length();

		FVector WallUpVelocity = WallUpVector * WallForwardMagnitude;
		FVector WallRightVelocity = Velocity.ProjectOnTo(-WallLeftVector);

		FVector MoveVelocity = WallUpVelocity + WallRightVelocity;

		FVector Delta = timeTick * MoveVelocity;
		bool bZeroDelta = Delta.IsNearlyZero();

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			FHitResult TraceResult;
			FRotator DeltaRotator = RotationRate * deltaTime;
		
			DrawDebugSphere(GetWorld(), GetActorLocation() + MoveVelocity, 25, 12, FColor::Green);

			SafeMoveUpdatedComponent(Delta, ComputeOrientToMovementRotation(GetActorTransform().Rotator(), deltaTime, DeltaRotator), false, TraceResult);

		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}

}

FRotator UGSCharacterMovementComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const
{
	FRotator NewRotator = Super::ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRotation);
	NewRotator.Pitch = GetActorTransform().Rotator().Pitch;
	NewRotator.Roll = GetActorTransform().Rotator().Roll;

	return NewRotator;
}

void UGSCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case WALLRUN:
		PhysWallRun(deltaTime,Iterations);
		break;
	}
}

void UGSCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkSpeedAtStart = MaxWalkSpeed;
}

bool UGSCharacterMovementComponent::TryAndStartWallRun()
{

	FVector StartLocation = GetActorLocation();

	FVector EndLocation = GetActorLocation() + (GetCurrentAcceleration().GetClampedToMaxSize(1) * 500);

	FCollisionQueryParams CollisionQueryParams;

	CollisionQueryParams.AddIgnoredActor(GetOwner());

	bool CanWallRun = false;

	if (GetWorld()->LineTraceSingleByObjectType(WallRunTrace,StartLocation,EndLocation,FCollisionObjectQueryParams::AllObjects,CollisionQueryParams))
	{
		CanWallRun = Cast<ABaseWallRunObject>(WallRunTrace.GetActor()) != nullptr;

		if (CanWallRun)
		{
			bIsWallRunning = true;

			//UE_LOG(LogTemp, Warning, TEXT("Wall Run Activated"));

			SetMovementMode(EMovementMode::MOVE_Custom, WALLRUN);
		}

	}

	if (!CanWallRun)
	{
		StopWallRun();
	}

	return CanWallRun;
}

void UGSCharacterMovementComponent::Sprint(bool CheckForWallRun)
{
	if (CheckForWallRun && TryAndStartWallRun())
	{
		StopSprint(false);
		return;
	}

	if (!IsWalking())
	{
		return;
	}

	if (bIsSprinting)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Sprint Activated"));

	bIsSprinting = true;
	MaxWalkSpeed = MaxSprintSpeed;

}

void UGSCharacterMovementComponent::StopSprint(bool CheckForWallRun)
{

	if (CheckForWallRun && bIsWallRunning)
	{
		StopWallRun();
		return;
	}

	if (bIsSprinting)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Sprint Deactivated"));

		bIsSprinting = false;
		MaxWalkSpeed = MaxWalkSpeedAtStart;
	}
}

void UGSCharacterMovementComponent::StopWallRun()
{
	if (bIsWallRunning)
	{
		//UE_LOG(LogTemp, Warning, TEXT("WallRun Deactivated"));

		bIsWallRunning = false;
		SetMovementMode(EMovementMode::MOVE_Walking);
	}
}
