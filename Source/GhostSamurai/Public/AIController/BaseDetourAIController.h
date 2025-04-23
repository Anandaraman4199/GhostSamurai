// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseDetourAIController.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTSAMURAI_API ABaseDetourAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseDetourAIController(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

protected:

	virtual void BeginPlay() override;

};
