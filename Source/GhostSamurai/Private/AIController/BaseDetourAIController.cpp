// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController/BaseDetourAIController.h"
#include "Navigation/CrowdFollowingComponent.h"

ABaseDetourAIController::ABaseDetourAIController(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ABaseDetourAIController::BeginPlay()
{
	
	Super::BeginPlay();

	UCrowdFollowingComponent* CrowdFollowingComponent = FindComponentByClass<UCrowdFollowingComponent>();

	if (CrowdFollowingComponent)
	{
		CrowdFollowingComponent->SetCrowdRotateToVelocity(false);
	}
}
