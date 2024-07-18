// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../AIControllerBase.h"
#include "GangplankController.generated.h"

UCLASS()
class GANGPLANK_API AGangplankController : public AAIControllerBase
{
	GENERATED_BODY()

public:
	AGangplankController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
