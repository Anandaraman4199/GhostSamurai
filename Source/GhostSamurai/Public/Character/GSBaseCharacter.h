// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GSBaseCharacter.generated.h"

UCLASS()
class GHOSTSAMURAI_API AGSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGSBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly)
	class UGSCharacterMovementComponent* GSCharacterMovementComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintImplementableEvent)
	void OnWallRunStarted();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWallRunEnded();

};
