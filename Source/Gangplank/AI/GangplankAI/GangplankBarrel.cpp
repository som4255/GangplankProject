// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankBarrel.h"

// Sets default values
AGangplankBarrel::AGangplankBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mCollision);
	mMesh->SetupAttachment(mCollision);

	mCollision->SetCollisionProfileName(TEXT("EnemySpawnObj"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mCollision->SetWorldScale3D(FVector(1.6f, 1.6f, 1.6f));
	mCollision->SetCapsuleHalfHeight(70.f);
	mCollision->SetCapsuleRadius(55.f);
	mMesh->SetRelativeLocation(FVector(0.f, 0.f, -69.f));

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
}

void AGangplankBarrel::OnConstruction(const FTransform& Transform)
{
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
		0.2f
	);

	return 0.0f;
}

// Called every frame
void AGangplankBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), GetActorLocation(),
		450.f,
		20, FColor::Red, false, 0.35f);

#endif
}


