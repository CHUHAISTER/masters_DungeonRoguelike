// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "EnemyCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->InitSphereRadius(5.0f);



    RootComponent = CollisionComponent;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(CollisionComponent); 
    Mesh->SetCollisionProfileName("NoCollision");

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1000.f;
    ProjectileMovement->MaxSpeed = 1000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f;


    if (CollisionComponent->OnComponentBeginOverlap.IsBound())
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap delegate is BOUND"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Overlap delegate is NOT bound!"));
    }
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(3.0f);

    CollisionComponent->UpdateOverlaps();


    CollisionComponent->SetNotifyRigidBodyCollision(true);
    /*CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
    CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);*/

    UE_LOG(LogTemp, Warning, TEXT("Projectile: My ObjectType=%d"), (int32)CollisionComponent->GetCollisionObjectType());

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;

}


