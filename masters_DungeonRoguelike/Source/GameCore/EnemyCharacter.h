

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"		
#include "GameFramework/Controller.h"
#include "Projectile.h"


#include "EnemyCharacter.generated.h"

UCLASS()
class GAMECORE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ChoosePatrolTarget();
	
	void SetPatrolArea(FVector PatrolMin, FVector PatrolMax);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;
	
	void KnockbackFromPlayer(const FVector& PlayerLocation, float KnockbackStrength);


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);



	UPROPERTY(EditAnywhere)
	FVector PatrolAreaMin;

	UPROPERTY(EditAnywhere)
	FVector PatrolAreaMax;

	UPROPERTY(EditAnywhere)
	float PatrolWaitTime = 2.0f;

	UPROPERTY(EditAnywhere)
	float PatrolSpeed = 600.0f;

private:
	FVector CurrentTarget;

	bool bHasTarget = false;

	bool bIsChasing = false;

	FTimerHandle PatrolTimerHandle;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void StartPatrolWait();
};
