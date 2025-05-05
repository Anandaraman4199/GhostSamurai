// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState/ANS_NiagaraAttached.h"
#include "Component/GSNiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UANS_NiagaraAttached::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{


	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor) 
	{ 
		return; 
	}
	SpawnedNiagara = Cast<UGSNiagaraComponent>(OwnerActor->AddComponentByClass(UGSNiagaraComponent::StaticClass(), false, FTransform::Identity, true));

	auto FoundComps = OwnerActor->GetComponentsByTag(ComponentClassToAttached, ComponentTag);

	if (FoundComps.IsEmpty())
	{
		return;
	}

	USceneComponent* FoundComp = Cast<USceneComponent>(FoundComps[0]);

	if (!FoundComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your fucking damnn Tag Or Component to attach is not found!!!"))

		return;
	}
	
	SpawnedNiagara->AttachToComponent(FoundComp, FAttachmentTransformRules::KeepRelativeTransform);

	if (!NiagaraSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set the fucking Niagara System Asset!!!"))

		return;
	}

	OwnerActor->RegisterAllComponents();

	SpawnedNiagara->SetAsset(NiagaraSystem, false);

	SpawnedNiagara->Activate(true);

}

void UANS_NiagaraAttached::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UANS_NiagaraAttached::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (!SpawnedNiagara)
	{
		return;
	}

	SpawnedNiagara->Deactivate();

	if (SecondsToDestroy > 0)
	{
		SpawnedNiagara->DestroyInSometime(SecondsToDestroy);
		return;
	}
	

	SpawnedNiagara->DestroyComponent();

}
