// Fill out your copyright notice in the Description page of Project Settings.


#include "SGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

UCharacterMovementComponent* characterMovement;
float crouchTimeDelay = 0;
float baseWalkSpeed, sprintMovementSpeed = 600;
bool isSprinting = false;


// Sets default values
ASGCharacter::ASGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ASGCharacter::BeginPlay()
{
	Super::BeginPlay();
	characterMovement = this->GetCharacterMovement();

	baseWalkSpeed = characterMovement->MaxWalkSpeed;
}

void ASGCharacter::MoveForward(float value)
{
	if ((Controller) && (value != 0))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);
	}
}

void ASGCharacter::MoveRight(float value)
{
	if ((Controller) && (value != 0))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, value);
	}
}

void ASGCharacter::Crouch()
{
	if (characterMovement->IsCrouching())
		ACharacter::UnCrouch();
	else if (!characterMovement->IsFalling())
		ACharacter::Crouch();
}

void ASGCharacter::UnCrouch()
{
	//ACharacter::UnCrouch();
}

void ASGCharacter::Jump()
{
	ACharacter::Jump();
	ACharacter::UnCrouch();
}

void ASGCharacter::DrawBow()
{
	
}

void ASGCharacter::ReleaseBow()
{

}

void ASGCharacter::Sprint()
{
	isSprinting = true;
	characterMovement->MaxWalkSpeed = sprintMovementSpeed;
}

void ASGCharacter::StopSprint()
{
	isSprinting = false;
}


// Called every frame
void ASGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (characterMovement->IsFalling() && characterMovement->IsCrouching())
	{
		ACharacter::UnCrouch();
	}
	
	if (!isSprinting && characterMovement->MaxWalkSpeed != baseWalkSpeed && InputComponent->GetAxisValue(TEXT("MoveForward")) == 0 && InputComponent->GetAxisValue(TEXT("MoveRight")) == 0)
		characterMovement->MaxWalkSpeed = baseWalkSpeed;
}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASGCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASGCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASGCharacter::UnCrouch);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASGCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookHorizontal", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookVertical", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &ASGCharacter::DrawBow);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &ASGCharacter::ReleaseBow);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASGCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASGCharacter::StopSprint);

}

