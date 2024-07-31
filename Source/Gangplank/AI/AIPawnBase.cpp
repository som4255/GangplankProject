// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPawnBase.h"

TObjectPtr<UDataTable>	AAIPawnBase::mEnemyDataTable;
TObjectPtr<UDataTable>	AAIPawnBase::mVoiceDataTable;

AAIPawnBase::AAIPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> EnemyDataTable(TEXT(
		"/Script/Engine.DataTable'/Game/DataTable/DT_EnemyInfo.DT_EnemyInfo'"));
	if (EnemyDataTable.Succeeded())
	{
		mEnemyDataTable = EnemyDataTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> VoiceDataTable(TEXT(
		"/Script/Engine.DataTable'/Game/DataTable/DT_SkillVoice.DT_SkillVoice'"));
	if (VoiceDataTable.Succeeded())
	{
		mVoiceDataTable = VoiceDataTable.Object;
	}

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	mCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	SetRootComponent(mCollision);
	mMesh->SetupAttachment(mCollision);
	mMovement->SetUpdatedComponent(mCollision);

	mCollision->SetCollisionProfileName(TEXT("Enemy"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mMesh->bReceivesDecals = false;
}

void AAIPawnBase::LoadAIData(FName Name)
{
	if (!IsValid(mEnemyDataTable))
		return;

	const FEnemyInfo* mEnemyInfo =  mEnemyDataTable->FindRow<FEnemyInfo>(Name, TEXT(""));
	
	if (mEnemyInfo)
	{
		mHP = mEnemyInfo->HP;
		mMoveSpeed = mEnemyInfo->MoveSpeed;

		if (IsValid(mEnemyInfo->Mesh))
		{
			mMesh->SetSkeletalMesh(mEnemyInfo->Mesh);
		}
	}
	
	mMovement->MaxSpeed = mMoveSpeed;

	mCollision->SetCapsuleHalfHeight(mEnemyInfo->CollisionHalfHeight);
	mCollision->SetCapsuleRadius(mEnemyInfo->CollisionRadius);

	mMesh->SetRelativeLocation(FVector(0.0, 0.0, -mEnemyInfo->CollisionHalfHeight));
	mMesh->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
	mMesh->SetRelativeScale3D(mEnemyInfo->MeshScale);
}

void AAIPawnBase::LoadVoiceData(FName Name)
{
	if (!IsValid(mVoiceDataTable))
		return;

	const FSkillVoice* mVoiceData = mVoiceDataTable->FindRow<FSkillVoice>(Name, TEXT(""));

	if (mVoiceData)
	{
		mSkillQVoice = mVoiceData->mSkillQVoice;
		mSkillWVoice = mVoiceData->mSkillWVoice;
		mSkillEVoice = mVoiceData->mSkillEVoice;
		mSkillRVoice = mVoiceData->mSkillRVoice;
		mExVoice = mVoiceData->mExVoice;
	}
}

void AAIPawnBase::OnConstruction(const FTransform& Transform)
{
}

// Called when the game starts or when spawned
void AAIPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAIPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

