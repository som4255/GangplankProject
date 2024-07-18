// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "../GangplankPawn.h"
#include "../GangplankAnimInstance.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkillQ.generated.h"


/**
 * 
 */
UCLASS()
class GANGPLANK_API UBTTask_SkillQ : public UBTTaskNode
{
	GENERATED_BODY()

public :
	UBTTask_SkillQ();

private :
	TObjectPtr<AGangplankPawn> mAIPawn;

	TObjectPtr<AActor> mTarget;

	AAIController* mController;

	TObjectPtr<class UGangplankAnimInstance> mAnimInst;

	UPROPERTY(Category = Component, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* mSkillQAnim;

protected:
	// �� Task�� ���۵ɶ� ȣ��ȴ�.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory);

	// �۾��� �ߴ��� �� ���´�.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory);

	// �� �����Ӹ��� �����Ѵ�.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds);

	// Task�� ����ɶ� �����Ѵ�.
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, EBTNodeResult::Type TaskResult);
	
};
