// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"

AAIControllerBase::AAIControllerBase()
{
}

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
}
