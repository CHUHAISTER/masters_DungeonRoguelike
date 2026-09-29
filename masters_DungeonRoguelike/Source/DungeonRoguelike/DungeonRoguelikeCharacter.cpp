// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonRoguelikeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameCore/EnemyCharacter.h"
#include "MathCombat/MathManager.h"
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADungeonRoguelikeCharacter



ADungeonRoguelikeCharacter::ADungeonRoguelikeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADungeonRoguelikeCharacter::OnOverlap);

	
}

void ADungeonRoguelikeCharacter::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{

	if (!OtherActor || OtherActor == this) return;

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
	if (Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping with player, casting succeeded!"));
		Enemy->KnockbackFromPlayer(GetActorLocation(), 400.0f);
		if (bCanBeDamaged)
		{
			this->TakeDamage(5.0f);
			bCanBeDamaged = false;

			// Запускаємо таймер на 1 секунду (або скільки треба)
			GetWorldTimerManager().SetTimer(
				DamageImmunityTimer,
				this,
				&ADungeonRoguelikeCharacter::ResetDamageImmunity,
				1.5f, 
				false
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to AEnemyCharacter failed!"));
	}


}

void ADungeonRoguelikeCharacter::ResetDamageImmunity()
{
	bCanBeDamaged = true;
}

void ADungeonRoguelikeCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	
	if (PC)
	{
		PC->GetUHealthBar()->SetHealthPercent(CurrentHealth / MaxHealth);
	}
	if (CurrentHealth <= 0.f)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Character is dead!"));
		PC->SaveExamplesToFile();
		UGameplayStatics::OpenLevel(this, FName("LooseMenuMap"));
	}
}

void ADungeonRoguelikeCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<ADRPlayerController>(GetController());
	if (PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PC is valid"));
		if (PC->GetUHealthBar())
		{
			UE_LOG(LogTemp, Warning, TEXT("Widget is valid"));
			PC->GetUHealthBar()->SetHealthPercent(1.0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Widget is NULL"));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADungeonRoguelikeCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADungeonRoguelikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonRoguelikeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonRoguelikeCharacter::Look);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ADungeonRoguelikeCharacter::ShootProjectile);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADungeonRoguelikeCharacter::ShootProjectile()
{
	if (!ProjectileClass) return;
	if (CountProjectile > 0) 
	{
		SpawnProjectile();
	}
	else {
		bCanBeDamaged = false;
		PC->SetPause(true);
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
		PC->StartTaskMenu();
		bCanBeDamaged = true;
	}
	
}

void ADungeonRoguelikeCharacter::SpawnProjectile()
{
	CountProjectile--;
	FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * 100.f + FVector(0, 0, 50.f);
	FRotator MuzzleRotation = GetControlRotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);
	if (Projectile)
	{
		FVector LaunchDir = MuzzleRotation.Vector();
		Projectile->FireInDirection(LaunchDir);
	}
}

void ADungeonRoguelikeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADungeonRoguelikeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
