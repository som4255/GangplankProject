// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GangplankSetBlackBoard.h"


UBTService_GangplankSetBlackBoard::UBTService_GangplankSetBlackBoard()
{
	NodeName = TEXT("GangplankSetBB");
	Interval = 0.1f;
}

void UBTService_GangplankSetBlackBoard::TickNode(UBehaviorTreeComponent& OwnerComp, 
	uint8* NodeMemory, float DeltaSeconds)
{
	//BT를 가지고있는 컨트롤러를 가져온다.
	AAIController* Controller = OwnerComp.GetAIOwner();

	//해당 컨트롤러로 작동되는 Pawn을 가져온다.
	AGangplankPawn* AIPawn = Cast<AGangplankPawn>(Controller->GetPawn());

	Controller->GetBlackboardComponent()->SetValueAsBool(
		TEXT("CanAttack"), AIPawn->GetCanAttack());

	int SkillCanUse = AIPawn->GetSkillCanUse();
	bool IsCanUse;

	IsCanUse = (SkillCanUse % 2) == 1;
	SkillCanUse /= 2;
	Controller->GetBlackboardComponent()->SetValueAsBool(
		TEXT("CanUseSkillQ"), IsCanUse);

	IsCanUse = (SkillCanUse % 2) == 1;
	SkillCanUse /= 2;

	IsCanUse = (SkillCanUse % 2) == 1;
	SkillCanUse /= 2;
	Controller->GetBlackboardComponent()->SetValueAsBool(
		TEXT("CanUseSkillE"), IsCanUse);
}
