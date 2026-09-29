


#include "EnemyCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "NavigationSystem.h"
#include <NavMesh/NavMeshBoundsVolume.h>
#include "Navigation/PathFollowingComponent.h"



// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 1000.0f;
	PawnSensingComponent->SetPeripheralVisionAngle(60.0f);
	PawnSensingComponent->bHearNoises = false;
	
	/*GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCapsuleComponent()->SetSimulatePhysics(false);
	GetCapsuleComponent()->SetEnableGravity(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);*/



	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIController::StaticClass();
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->UpdateOverlaps();
	UE_LOG(LogTemp, Warning, TEXT("Enemy Capsule: ObjectType=%d, OverlapEvents=%d"),
		(int32)GetCapsuleComponent()->GetCollisionObjectType(),
		(int32)GetCapsuleComponent()->GetGenerateOverlapEvents());

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlap);


	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("No Controller is set for the character!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is set and ready!"));
	}
	/*if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
	}*/

	if (GetCharacterMovement())
	{
		
		GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed; 
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterMovementComponent is missing!"));
	}

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

	ChoosePatrolTarget();

	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*auto* Move = GetCharacterMovement();
	UE_LOG(LogTemp, Warning, TEXT("Speed: %f | Velocity: %s | IsFalling: %s"),
		Move->Velocity.Size(),
		*Move->Velocity.ToString(),
		Move->IsFalling() ? TEXT("Yes") : TEXT("No"));*/

	if (!bHasTarget) return;

	FVector Direction = (CurrentTarget - GetActorLocation()).GetSafeNormal2D();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation ProjectedLocation;
	

	
	if (NavSystem && NavSystem->ProjectPointToNavigation(CurrentTarget, ProjectedLocation))
	{
		CurrentTarget = ProjectedLocation.Location;
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController && AIController->GetPathFollowingComponent())
		{

			FAIMoveRequest MoveRequest;
			MoveRequest.SetAcceptanceRadius(5.0f);
			MoveRequest.SetGoalLocation(CurrentTarget);

			FNavPathSharedPtr NavPath;
			EPathFollowingRequestResult::Type MoveResult = AIController->MoveTo(MoveRequest, &NavPath);

			
		}

	}


	float Dist2D = FVector::Dist2D(GetActorLocation(), CurrentTarget);
	if (Dist2D < 100.f)
	{
		bHasTarget = false;
		if (!bIsChasing)
		{
			StartPatrolWait();
		}
	}

}

void AEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	bIsChasing = true;
	bHasTarget = true;
	CurrentTarget = Pawn->GetActorLocation();

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed * 1.5f;
}

void AEnemyCharacter::KnockbackFromPlayer(const FVector& PlayerLocation, float KnockbackStrength)
{
	FVector Direction = GetActorLocation() - PlayerLocation;
	Direction.Z = 0.0f; 
	Direction.Normalize();

	FVector KnockbackImpulse = Direction * KnockbackStrength;
	UE_LOG(LogTemp, Warning, TEXT("KnockbackFromPlayer"));

	ACharacter* EnemyChar = Cast<ACharacter>(this);
	if (EnemyChar)
	{
		FVector LaunchVelocity = KnockbackImpulse;
		LaunchVelocity.Z = 300.0f; 
		EnemyChar->LaunchCharacter(LaunchVelocity, true, true);
	}
	else
	{
		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GetRootComponent());
		if (RootComp && RootComp->IsSimulatingPhysics())
		{
			RootComp->AddImpulse(KnockbackImpulse, NAME_None, true);
		}
	}
}

void AEnemyCharacter::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (!OtherActor || OtherActor == this) return;

	if (OtherActor->IsA(AProjectile::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping Projectile: %s (ENEMY)"), *OtherActor->GetName());
		OtherActor->Destroy();
		Destroy();

	}


}

void AEnemyCharacter::ChoosePatrolTarget()
{


	FVector Origin = GetActorLocation();
	float Z = Origin.Z;

	FVector NewTarget;
	const float MinDistance = 300.0f; 

	int32 Attempts = 10;
	while (Attempts-- > 0)
	{
		float X = FMath::FRandRange(PatrolAreaMin.X-1, PatrolAreaMax.X-1);
		float Y = FMath::FRandRange(PatrolAreaMin.Y-1, PatrolAreaMax.Y-1);
		NewTarget = FVector(X, Y, Z);

		if (FVector::Dist2D(Origin, NewTarget) >= MinDistance)
			break;
	}

	CurrentTarget = NewTarget;
	bHasTarget = true;
	bIsChasing = false;

	//DrawDebugSphere(GetWorld(), CurrentTarget, 30.f, 12, FColor::Blue, false, 45.f);
	

}

void AEnemyCharacter::SetPatrolArea(FVector PatrolMin, FVector PatrolMax)
{
	
	PatrolAreaMin = PatrolMin;
	PatrolAreaMax = PatrolMax;
}

void AEnemyCharacter::StartPatrolWait()
{
	GetWorldTimerManager().SetTimer(
		PatrolTimerHandle,
		this,
		&AEnemyCharacter::ChoosePatrolTarget,
		PatrolWaitTime,
		false
	);
}


// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


