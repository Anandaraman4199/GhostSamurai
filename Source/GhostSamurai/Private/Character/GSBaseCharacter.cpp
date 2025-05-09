// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GSBaseCharacter.h"
#include "Component/GSCharacterMovementComponent.h"

// Sets default values
AGSBaseCharacter::AGSBaseCharacter(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer.SetDefaultSubobjectClass<UGSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GSCharacterMovementComp = Cast<UGSCharacterMovementComponent>(GetCharacterMovement());

}

// Called when the game starts or when spawned
void AGSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

