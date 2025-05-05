// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState/ANS_NiagaraAttached.h"
#include "Component/GSNiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UANS_NiagaraAttached::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	SpawnedNiagara = Cast<UGSNiagaraComponent>(MeshComp->GetOwner()->AddComponentByClass(UGSNiagaraComponent::StaticClass(), false, FTransform::Identity, true));
}

void UANS_NiagaraAttached::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UANS_NiagaraAttached::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
