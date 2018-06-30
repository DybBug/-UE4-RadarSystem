// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RadarSystemCharacter.generated.h"

UCLASS(config=Game)
class ARadarSystemCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARadarSystemCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	float m_BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	float m_BaseLookUpRate;

protected :
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* m_pCameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* m_pFollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Configuration|Components")
	class UComponent_Radar* m_pRadar;

public:

	/* Get */
	FORCEINLINE class USpringArmComponent* const& GetCameraBoom()   const { return m_pCameraBoom; }
	FORCEINLINE class UCameraComponent*    const& GetFollowCamera() const { return m_pFollowCamera; }
	FORCEINLINE class UComponent_Radar*    const& GetRadar()        const { return m_pRadar; }

protected:
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

protected :

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	UFUNCTION()
	void _CKey();

	UFUNCTION()
	void _OneKey();
};

