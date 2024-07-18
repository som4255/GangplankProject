// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankPawn.h"
#include "math.h"

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

void AGangplankPawn::LoadAIData(FName Name)
{
	Super::LoadAIData(Name);
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

void AGangplankPawn::Attack_SkillE()
{
	mNavMesh = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (mNavMesh == nullptr)
		return;

	TArray<FVector> BarrelLoc;
	FNavLocation NavRandomLoc;

	FActorSpawnParameters	ActorParam;
	ActorParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	while(BarrelLoc.Num() < 3)
	{
		if (mNavMesh->GetRandomPointInNavigableRadius(GetActorLocation(), 2000.f, NavRandomLoc))
		{
			BarrelLoc.Add(NavRandomLoc.Location);
		}
	}
	
	for (auto& SpawnLoc : BarrelLoc)
	{
		SpawnLoc += FVector(0.f, 0.f, 107.f);

		if(IsValid(mBarrelClass))
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

void AGangplankPawn::Attack_SkillQ()
{
	int32 RandNum = FMath::RandRange(0, mSkillQVoice.Num() - 1);

	UGameplayStatics::PlaySound2D(
		GetWorld(),
		mSkillQVoice[RandNum],
		0.5f
	);

	FActorSpawnParameters	ActorParam;
	ActorParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator BulletRot = FRotator::ZeroRotator;

	if (IsValid(Super::mTargetActor))
	{
		BulletRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Super::mTargetActor->GetActorLocation());
	}
	
	if (IsValid(mBulletClass))
		GetWorld()->SpawnActor<AGangplankBullet>(
			mBulletClass,
			mMesh->GetSocketLocation("MuzzleSocket"),
			BulletRot, 
			ActorParam
		);
}

void AGangplankPawn::AddCooldownTimer(float Time, FName Name)
{
	//구조체의 크기가 작고 함수의 스코프를 벗어날 시 삭제됭도 무방하기 때문에
	//new를 사용하여 힙 영역에 생성하는 것이 아닌 스택 영역에 생성하는 것이 더 효율적임
	FCooldownTimer NEWTIMER;

	NEWTIMER.TimerDele.BindUFunction(mTimerComponent, FName(Name));
	NEWTIMER.CoolTime = Time;

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

	AddCooldownTimer(5.f, "SkillQTimer");
	AddCooldownTimer(10.f, "SkillWTimer");
	AddCooldownTimer(12.f, "SkillETimer");
	AddCooldownTimer(100.f, "SkillRTimer");
}
