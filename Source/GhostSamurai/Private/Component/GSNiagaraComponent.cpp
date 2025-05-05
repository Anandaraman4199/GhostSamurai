// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GSNiagaraComponent.h"

UGSNiagaraComponent::UGSNiagaraComponent(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

void UGSNiagaraComponent::DestroyInSometime(float SometimeInSeconds)
{
	FTimerDelegate TimerDelegate;
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandleToDestroy, this, &UGSNiagaraComponent::Destroy_Internal, SometimeInSeconds, false);
}

void UGSNiagaraComponent::Destroy_Internal()
{
	DestroyComponent(true);
}
