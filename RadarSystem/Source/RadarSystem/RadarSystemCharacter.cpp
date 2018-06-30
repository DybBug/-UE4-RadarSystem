// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RadarSystemCharacter.h"
#include "Components/Component_Radar.h"

#include <HeadMountedDisplayFunctionLibrary.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/InputComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>
#include <Engine/World.h>

//////////////////////////////////////////////////////////////////////////
// ARadarSystemCharacter

ARadarSystemCharacter::ARadarSystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	m_BaseTurnRate = 45.f;
	m_BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	m_pCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_pCameraBoom->SetupAttachment(RootComponent);
	m_pCameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	m_pCameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												   // Create a follow camera
	m_pFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_pFollowCamera->SetupAttachment(m_pCameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_pFollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

													  // Create a Radar;
	m_pRadar = CreateDefaultSubobject<UComponent_Radar>(TEXT("Radar"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ARadarSystemCharacter::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;	
	CastChecked<APlayerController>(GetController())->SetInputMode(InputMode);

	m_pRadar->InitializeRadar(this);
	m_pRadar->OnBecomeVisible();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARadarSystemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARadarSystemCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARadarSystemCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ARadarSystemCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ARadarSystemCharacter::LookUpAtRate);

	PlayerInputComponent->BindKey(EKeys::C,   IE_Pressed, this, &ARadarSystemCharacter::_CKey);
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ARadarSystemCharacter::_OneKey);
}

void ARadarSystemCharacter::TurnAtRate(float Rate)
{
	if (Rate != 0.f)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * m_BaseTurnRate * GetWorld()->GetDeltaSeconds());
		m_pRadar->OnPlayerTurned();
	}
}

void ARadarSystemCharacter::LookUpAtRate(float Rate)
{
	if (Rate != 0)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * m_BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ARadarSystemCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		m_pRadar->OnPlayerMoved();
	}
}

void ARadarSystemCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		m_pRadar->OnPlayerMoved();
	}
}

void ARadarSystemCharacter::_CKey()
{
	m_pRadar->ToggleVisibility(true);
}

void ARadarSystemCharacter::_OneKey()
{
	m_pRadar->RemoveMarkerAtIndex(0);
}



