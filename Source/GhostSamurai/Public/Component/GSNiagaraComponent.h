// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GSNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTSAMURAI_API UGSNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	void DestroyInSometime(float SometimeInSeconds);
	
	FTimerHandle TimerHandleToDestroy;
};
