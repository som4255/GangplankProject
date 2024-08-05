// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AIFindTarget.h"
#include "../../AIPawnBase.h"

UBTService_AIFindTarget::UBTService_AIFindTarget()
{
	NodeName = TEXT("AIFindTarget");
	Interval = 3.f;
	RandomDeviation = 1.f;
}

void UBTService_AIFindTarget::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BT�� �������ִ� ��Ʈ�ѷ��� �����´�.
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	//�ش� ��Ʈ�ѷ��� �۵��Ǵ� Pawn�� �����´�.
	AAIPawnBase* AIPawn = Cast<AAIPawnBase>(Controller->GetPawn());
	
	//Ž���� ���� �⺻����
	FVector AILoc = AIPawn->GetActorLocation();
	TArray<FHitResult> result;
	FCollisionQueryParams	param(NAME_None, false, AIPawn);
	
	bool Collision = GetWorld()->SweepMultiByChannel(result,
		AILoc, AILoc,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(5000.f),
		param);

	if (Collision)
	{
		// Controller�κ��� Blackboard�� ���ͼ� Target�� �����Ѵ�.
		// FHitResult�� GetActor() �Լ��� �浹�� ���͸� �������� �Լ��̴�.
		Controller->GetBlackboardComponent()->SetValueAsObject(
			TEXT("Target"), result[0].GetActor());

		AIPawn->SetTarget(result[0].GetActor());
	}

	else
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(
			TEXT("Target"), nullptr);
	}
}
