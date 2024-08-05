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

	//BT를 가지고있는 컨트롤러를 가져온다.
	AAIController* Controller = OwnerComp.GetAIOwner();
	
	//해당 컨트롤러로 작동되는 Pawn을 가져온다.
	AAIPawnBase* AIPawn = Cast<AAIPawnBase>(Controller->GetPawn());
	
	//탐지를 위한 기본세팅
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
		// Controller로부터 Blackboard를 얻어와서 Target을 지정한다.
		// FHitResult의 GetActor() 함수는 충돌된 액터를 가져오는 함수이다.
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
