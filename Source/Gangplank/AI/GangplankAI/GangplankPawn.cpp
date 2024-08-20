// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankPawn.h"

AGangplankPawn::AGangplankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	mName = "Gangplank";

	AIControllerClass = AGangplankController::StaticClass();
	mBarrelClass = AGangplankBarrel::StaticClass();
	mBulletClass = AGangplankBullet::StaticClass();

	

	mTimerComponent = CreateDefaultSubobject<UGangplankCooldownTimer>(TEXT("TimerComponent"));

	static ConstructorHelpers::FClassFinder<UAnimInstance>	
		AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Gangplank/AB_Gangplank.AB_Gangplank_C'"));

	if (AnimAsset.Succeeded())
	{
		mMesh->SetAnimInstanceClass(AnimAsset.Class);	
	}

	LoadAIData(mName);
	LoadVoiceData(mName);
}

void AGangplankPawn::SetCoolTime(EGangplankSkill Skill)
{
	if (Skill + 1 > mCooldownTimer.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, FString::Printf(TEXT("mCooldownTimerOutBound")));
		return;
	}

	float i = Skill;
	mSkillCanUse -= pow(2.f, i);

	//델리게이트를 이용한 타이머 생성
	GetWorld()->GetTimerManager().SetTimer(
		mCooldownTimer[Skill].TimerHandle,
		mCooldownTimer[Skill].TimerDele,
		1.f, //0은 작동을 안한다 찾아보니 언리얼이 InRate값이 0인 경우는 지원히지 않는다고 한다.
		false,
		mCooldownTimer[Skill].CoolTime
	);
}

void AGangplankPawn::Attack_SkillQ()
{
	int32 RandNum = FMath::RandRange(0, mSkillQVoice.Num() - 1);

	UGameplayStatics::PlaySound2D(
		GetWorld(),
		mSkillQVoice[RandNum],
		0.5f
	);

	//Bullet Spawn
	FActorSpawnParameters	ActorParam;
	ActorParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (IsValid(mBulletClass))
	{
		GetWorld()->SpawnActor<AGangplankBullet>(
			mBulletClass,
			mMesh->GetSocketLocation("MuzzleSocket"),
			mSkillQTargetRot,
			ActorParam
		);

		if(mAdditionalBarrelLoc!=FVector::ZeroVector)
			Attack_SkillE_Additional();
	}
}

void AGangplankPawn::Attack_SkillE()
{
	if (mNavMesh == nullptr)
		return;

	TArray<FVector> BarrelLoc;
	FNavLocation NavRandomLoc;

	FActorSpawnParameters	ActorParam;
	ActorParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//mNavMesh->GetRandomReachablePointInRadius : AI가 갈 수 있는 랜덤 위치
	//mNavMesh->GetRandomPointInNavigableRadius : 네비게이션 메시 상의 랜덤 위치

	while (BarrelLoc.Num() < 3)
	{
		if (mNavMesh->GetRandomReachablePointInRadius(GetActorLocation(), 5000.f, NavRandomLoc))
		{
			BarrelLoc.Add(NavRandomLoc.Location);
		}
	}

	for (auto& SpawnLoc : BarrelLoc)
	{
		SpawnLoc += FVector(0.f, 0.f, BARRELHEIGHT);

		if (IsValid(mBarrelClass))
			GetWorld()->SpawnActor<AGangplankBarrel>(mBarrelClass,
				SpawnLoc, FRotator::ZeroRotator, ActorParam);
	}

	if (EExVoice::Barrel <= mExVoice.Num())
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			mExVoice[EExVoice::Barrel],
			0.5f
		);
	}
}

void AGangplankPawn::Attack_SkillE_Additional()
{
	FActorSpawnParameters	ActorParam;
	ActorParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FNavLocation NavRandomLoc;

	TArray<FHitResult> result;
	FCollisionQueryParams	param;

	for(int i=0; i<30; i++)
	{
		if (mNavMesh->GetRandomReachablePointInRadius(mAdditionalBarrelLoc, 900.f, NavRandomLoc))
		{
			bool Collision = GetWorld()->SweepMultiByChannel(
				result, NavRandomLoc, NavRandomLoc,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel2,
				FCollisionShape::MakeSphere(400.f),
				param);

			if (Collision && IsValid(mBarrelClass))
			{
				GetWorld()->SpawnActor<AGangplankBarrel>(mBarrelClass,
					NavRandomLoc.Location, FRotator::ZeroRotator, ActorParam);

				return;
			}
		}
	}	
}

void AGangplankPawn::AddCooldownTimer(float Time, FName Name)
{
	//TArray타입의 mCooldownTimer에 Add 것이기 때문에 동적 할당이 아닌
	//함수 내 지역 변수로 선언한 뒤 mCooldownTimer에 Add해줌
	//TObjectPtr타입을 TArray에 넣으려 할 경우 오류가 발생하여 이렇게 구현함
	FCooldownTimer NEWTIMER;

	//BindUFunction(바인드할 클래스, 클래스 내의 함수 이름)
	NEWTIMER.TimerDele.BindUFunction(mTimerComponent, FName(Name));
	NEWTIMER.CoolTime = Time;

	//mCooldownTimer가 TArray타입이기 때문에 Add시 NEWTIMER의 복사본을 생성하여 힙 영역에 저장
	//함수 종료 후 NEWTIMER가 삭제되어도 복사본은 힙 영역에 저장되어 있기 때문에 접근 가능
	mCooldownTimer.Add(NEWTIMER);
}

void AGangplankPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AGangplankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGangplankPawn::BeginPlay()
{
	Super::BeginPlay();

	mAnim = Cast<UGangplankAnimInstance>(mMesh->GetAnimInstance());
	mNavMesh = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	AddCooldownTimer(5.f, "SkillQTimer");
	AddCooldownTimer(10.f, "SkillWTimer");
	AddCooldownTimer(12.f, "SkillETimer");
	AddCooldownTimer(100.f, "SkillRTimer");
}
