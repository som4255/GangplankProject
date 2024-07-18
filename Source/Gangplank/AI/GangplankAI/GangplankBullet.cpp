// Fill out your copyright notice in the Description page of Project Settings.


#include "GangplankBullet.h"

// Sets default values
AGangplankBullet::AGangplankBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	mNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	SetRootComponent(mCollision);
	mMesh->SetupAttachment(mCollision);
	mNiagara->SetupAttachment(mCollision);
	
	mCollision->SetCollisionProfileName(TEXT("EnemyProjectile"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mNiagara->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	mCollision->SetSphereRadius(10.f);
	mMesh->SetRelativeScale3D(FVector(0.1f, 0.3f, 0.3f));
	mMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	mNiagara->SetRelativeScale3D(FVector(0.35f, 0.5f, 0.5f));
	mNiagara->SetRelativeLocation(FVector(-35.f, 0.f, 0.f));
	
	mProjectile->InitialSpeed = 4000.f;
	mProjectile->MaxSpeed = 4000.f;
	mProjectile->ProjectileGravityScale = 0.f;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/Gangplank/Bullet/Resource/BulletHead.BulletHead'"));
	if (MeshAsset.Succeeded())
	{
		mMesh->SetStaticMesh(MeshAsset.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/Gangplank/Bullet/NS_Bullet.NS_Bullet'"));
	if (Niagara.Succeeded())
	{
		mNiagara->SetAsset(Niagara.Object);
	}
}

// Called when the game starts or when spawned
void AGangplankBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGangplankBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeadCount += DeltaTime;

	if (DeadCount >= 2.f)
		Destroy();
}

void AGangplankBullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
	UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, 
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	FDamageEvent DmgEvent;
	Other->TakeDamage(20.f, DmgEvent, nullptr, this);

	Destroy();
}

