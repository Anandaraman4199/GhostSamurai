// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GSCharacterMovementComponent.h"
#include "Actors/BaseWallRunObject.h"
#include "Character/GSBaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

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
	MaxCustomMovementSpeed = MaxWallRunSpeed;

	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		const FVector OldLocation = UpdatedComponent->GetComponentLocation();

		FVector PlayerInput = GetCurrentAcceleration();
		PlayerInput.Normalize();

		float PlayerInputDot = FVector::DotProduct(PlayerInput, WallRunTrace.Normal);

		if (PlayerInputDot > 0.6f || GetCurrentAcceleration().IsNearlyZero(10))
		{
			StopWallRun();
			break;
		}


		CalcVelocity(timeTick, WallRunFriction, false, BrakingDecelerationWallRun);

		// Compute move parameters
		FVector ActorMoveVelocity = Velocity;

		FVector WallBackwardVector = WallRunTrace.Normal;
		FVector approxWallLeftVector = FVector::CrossProduct(WallBackwardVector, GetOwner()->GetActorUpVector());
		FVector WallUpVector = FVector::CrossProduct(approxWallLeftVector, WallBackwardVector);
		FVector WallLeftVector = FVector::CrossProduct(WallUpVector, WallBackwardVector);

		TowardsWallRotation = UKismetMathLibrary::MakeRotationFromAxes(-WallBackwardVector, -WallLeftVector, WallUpVector);

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

			FHitResult DistanceCheckTraceResult;
			FVector StartLocation = GetActorLocation();

			FVector EndLocation = GetActorLocation() + (-WallRunTrace.Normal * 1000);

			FCollisionQueryParams CollisionQueryParams;

			CollisionQueryParams.AddIgnoredActor(GetOwner());


			if (GetWorld()->LineTraceSingleByObjectType(DistanceCheckTraceResult, StartLocation, EndLocation, FCollisionObjectQueryParams::AllObjects, CollisionQueryParams))
			{
				if (Cast<ABaseWallRunObject>(DistanceCheckTraceResult.GetActor()))
				{
					float NewDist = WallRunMaintainDistance - DistanceCheckTraceResult.Distance;

					SafeMoveUpdatedComponent(NewDist * DistanceCheckTraceResult.Normal, GetActorTransform().Rotator(), false, TraceResult);

				}
			}
			else
			{
				StopWallRun();
				CantWallRunFor(0.5f);
				PerformWallRunEndJump();
				return;
			}

			FRotator DR = GetDeltaRotation(timeTick);

			ComputeOrientToMovementRotation(GetActorTransform().Rotator(), timeTick, DR);

			if (!SafeMoveUpdatedComponent(Delta, GetActorTransform().Rotator(), true, TraceResult))
			{
				CantWallRunFor(0.5f);
				StopWallRun();
				return;
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
		//Test code to get character wallrunning direction

		FVector MoveVelocityNoramlaized = MoveVelocity;
		MoveVelocityNoramlaized.Normalize();

		float verticalComponent = FVector::DotProduct(MoveVelocityNoramlaized, WallUpVector);
		float HorizontalComponent= FVector::DotProduct(MoveVelocityNoramlaized, WallLeftVector);
		
		
		CharacterWallRunDirection = HorizontalComponent;
		
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

float UGSCharacterMovementComponent::GetMaxAcceleration() const
{
	if (IsWallRunning())
	{
		return MaxWallRunAcceleration;
	}
	return MaxAcceleration;
}

void UGSCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkSpeedAtStart = MaxWalkSpeed;

}

bool UGSCharacterMovementComponent::TryAndStartWallRun()
{
	if (bPreventWallRun)
	{
		return false;
	}

	if (bIsWallRunning)
	{
		return true;
	}

	FVector StartLocation = GetActorLocation();

	FVector EndLocation = GetActorLocation() + (GetCurrentAcceleration().GetClampedToMaxSize(1) * WallRunDetectionDistance);

	FCollisionQueryParams CollisionQueryParams;

	CollisionQueryParams.AddIgnoredActor(GetOwner());

	bool CanWallRun = false;

	if (GetWorld()->LineTraceSingleByObjectType(WallRunTrace,StartLocation,EndLocation,FCollisionObjectQueryParams::AllObjects,CollisionQueryParams))
	{

		CanWallRun = Cast<ABaseWallRunObject>(WallRunTrace.GetActor()) != nullptr;

		if (CanWallRun)
		{

			float WallNormalDot = FVector::DotProduct(WallRunTrace.Normal, GetGravityDirection());
			float ViewDot = FVector::DotProduct(WallRunTrace.Normal, UKismetMathLibrary::GetForwardVector(GetCharacterOwner()->GetControlRotation()));

			if (UKismetMathLibrary::InRange_FloatFloat(WallNormalDot, -0.3f, 0.05) && ViewDot < -0.95f)
			{

				bIsWallRunning = true;

				//UE_LOG(LogTemp, Warning, TEXT("Wall Run Activated"));

				SetMovementMode(EMovementMode::MOVE_Custom, WALLRUN);

				Cast<AGSBaseCharacter>(GetOwner())->OnWallRunStarted();
			}

		}

	}

	return CanWallRun;
}

void UGSCharacterMovementComponent::StartSprint(bool CheckForWallRun)
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

		Cast<AGSBaseCharacter>(GetOwner())->OnWallRunEnded();
	}
}

void UGSCharacterMovementComponent::PerformWallRunEndJump()
{
	
}

void UGSCharacterMovementComponent::CantWallRunFor(float seconds)
{
	bPreventWallRun = true;
	GetWorld()->GetTimerManager().SetTimer(PreventWallRunTimerHandle, this, &UGSCharacterMovementComponent::StopPreventWallRun, seconds);
}

void UGSCharacterMovementComponent::StopPreventWallRun()
{
	bPreventWallRun = false;
	GetWorld()->GetTimerManager().ClearTimer(PreventWallRunTimerHandle);
}
