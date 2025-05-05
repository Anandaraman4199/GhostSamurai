// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GSNiagaraComponent.h"

void UGSNiagaraComponent::DestroyInSometime(float SometimeInSeconds)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DestroyComponent"), true);
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandleToDestroy, TimerDelegate, SometimeInSeconds, false);
}
