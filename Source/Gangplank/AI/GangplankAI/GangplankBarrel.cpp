// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankBarrel.h"

// Sets default values
AGangplankBarrel::AGangplankBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mAttackPreview = CreateDefaultSubobject<UDecalComponent>(TEXT("AttackPreview"));

	SetRootComponent(mCollision);
	mMesh->SetupAttachment(mCollision);
	mAttackPreview->SetupAttachment(mCollision);

	mCollision->SetCollisionProfileName(TEXT("EnemySpawnObj"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollision->SetWorldScale3D(FVector(1.6f, 1.6f, 1.6f));
	mCollision->SetCapsuleHalfHeight(70.f);
	mCollision->SetCapsuleRadius(55.f);
	mMesh->SetRelativeLocation(FVector(0.f, 0.f, -69.f));
	mAttackPreview->SetWorldScale3D(FVector(0.05f, 1.1f, 1.1f));
	mAttackPreview->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	mAttackPreview->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Gangplank/Barrel/Gangplank_barrel.Gangplank_barrel'"));
	if (Mesh.Succeeded())
	{
		mMesh->SetSkeletalMesh(Mesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Gangplank/Barrel/AB_GangplankBarrel.AB_GangplankBarrel_C'"));

	if (AnimAsset.Succeeded())
	{
		mMesh->SetAnimInstanceClass(AnimAsset.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>
		DecalAsset(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Gangplank/Barrel/Materials/MI_BarrelAttackPreview.MI_BarrelAttackPreview'"));

	if (DecalAsset.Succeeded())
	{
		mAttackPreview->SetDecalMaterial(DecalAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/Gangplank/Barrel/NS_BarrelConnection.NS_BarrelConnection'"));
	if (Niagara.Succeeded())
	{
		mBarrelConnection = Niagara.Object;
	}
}

void AGangplankBarrel::OnConstruction(const FTransform& Transform)
{
	mBarrelConnectionArray.Empty();

	TArray<FHitResult> result;
	FCollisionQueryParams	param(NAME_None, false, this);
	FRotator mFireRot = FRotator::ZeroRotator;

	bool Collision = GetWorld()->SweepMultiByChannel(
		result,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(900.f),
		param
	);

	if (Collision)
	{
		for (auto& barrel : result)
		{
			TObjectPtr<UNiagaraComponent> NEWNIAGARA = 
				NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), TEXT("BarrelConnection"));

			NEWNIAGARA->SetAsset(mBarrelConnection);
			NEWNIAGARA->SetVariableVec3(FName("TargetLoc"), barrel.Location);
			NEWNIAGARA->SetupAttachment(mCollision);
			mBarrelConnectionArray.Add(NEWNIAGARA);
		}
	}
}

void AGangplankBarrel::Explode()
{
	TArray<TObjectPtr<AActor>> IgnoreActors;

	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		30.f,
		GetActorLocation(),
		450.f,
		nullptr,
		IgnoreActors,
		this,
		nullptr,
		false,
		ECollisionChannel::ECC_GameTraceChannel8
	);

	Destroy();
}

// Called when the game starts or when spawned
void AGangplankBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

float AGangplankBarrel::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	mCollision->SetCollisionProfileName("NoCollision");

	GetWorld()->GetTimerManager().SetTimer(
		ExplodeTimer,
		this,
		&AGangplankBarrel::Explode,
		1.f, //0은 작동을 안한다 찾아보니 언리얼이 InRate값이 0인 경우는 지원히지 않는다고 한다.
		false,
		0.4f
	);

	return 0.0f;
}

// Called every frame
void AGangplankBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//#if ENABLE_DRAW_DEBUG
//DrawDebugSphere(GetWorld(), GetActorLocation(),
//	450.f,
//	20, FColor::Red, false, 0.35f);
//
//#endif
}


