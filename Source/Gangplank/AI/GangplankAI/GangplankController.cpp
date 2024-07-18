// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankController.h"

AGangplankController::AGangplankController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITree(TEXT("/Script/AIModule.BehaviorTree'/Game/Gangplank/BT_Gangplank.BT_Gangplank'"));

	if (AITree.Succeeded())
		mAITree = AITree.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>	AIBlackboard(TEXT("/Script/AIModule.BlackboardData'/Game/Gangplank/BB_Gangplank.BB_Gangplank'"));

	if (AIBlackboard.Succeeded())
		mAIBlackboard = AIBlackboard.Object;
}

void AGangplankController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(mAITree) && IsValid(mAIBlackboard))
	{
		UBlackboardComponent* BlackboardRef = Blackboard;
		if (UseBlackboard(mAIBlackboard, BlackboardRef))
		{
			RunBehaviorTree(mAITree);
		}
	}

}

void AGangplankController::OnUnPossess()
{
	Super::OnUnPossess();
}
