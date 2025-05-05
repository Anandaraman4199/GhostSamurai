// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GSNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Rendering, Common), Blueprintable, hidecategories = Object, hidecategories = Physics, hidecategories = Collision, showcategories = Trigger, editinlinenew, meta = (BlueprintSpawnableComponent, DisplayName = "GS Niagara Particle System Component"))

class GHOSTSAMURAI_API UGSNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	UGSNiagaraComponent(const FObjectInitializer& ObjectInitializer);

	void DestroyInSometime(float SometimeInSeconds);
	
	UFUNCTION()
	void Destroy_Internal();
	
	FTimerHandle TimerHandleToDestroy;
};
