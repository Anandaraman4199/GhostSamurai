// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_NiagaraAttached.generated.h"

/**
 * 
 */
UCLASS()
class GHOSTSAMURAI_API UANS_NiagaraAttached : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<USceneComponent> ComponentClassToAttached;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ComponentTag;

private:

	class UGSNiagaraComponent* SpawnedNiagara;

};
