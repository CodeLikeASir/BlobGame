// © Jan Meissner

#include "Blob_SawTrap.h"
#include "Components/SplineComponent.h"

// Sets default values
ABlob_SawTrap::ABlob_SawTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;

	SawBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SawBaseMesh"));
	SawBaseMesh->SetupAttachment(SplineComponent);

	SawBladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SawBladeMesh"));
	SawBladeMesh->SetupAttachment(SawBaseMesh);
}

// Called when the game starts or when spawned
void ABlob_SawTrap::BeginPlay()
{
	Super::BeginPlay();

	// Cache the total spline length
	SplineLength = SplineComponent->GetSplineLength();
    
	// Set initial position
	if (SplineLength > 0.0f)
	{
		FVector InitialLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
		SawBaseMesh->SetWorldLocation(InitialLocation);
        
		if (bRotateAlongSpline)
		{
			FVector Direction = SplineComponent->GetDirectionAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
			FRotator InitialRotation = Direction.Rotation();
			SawBaseMesh->SetWorldRotation(InitialRotation);
		}
	}
}

// Called every frame
void ABlob_SawTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (SplineLength <= 0.0f)
    {
        return;
    }
    
    // Update movement along spline
    float MovementDelta = MovementSpeed * DeltaTime * DirectionMultiplier;
    
    if (bReverseDirection)
    {
        MovementDelta *= -1.0f;
    }
    
    CurrentDistance += MovementDelta;
    
    // Handle looping and bouncing
    if (SplineComponent->IsClosedLoop())
    {
        // For closed loops, wrap around
        while (CurrentDistance >= SplineLength)
        {
            CurrentDistance -= SplineLength;
        }
        while (CurrentDistance < 0.0f)
        {
            CurrentDistance += SplineLength;
        }
    }
    else
    {
        // For open splines, bounce back and forth if looping is enabled
        if (bShouldLoop)
        {
            if (CurrentDistance >= SplineLength)
            {
                CurrentDistance = SplineLength;
                DirectionMultiplier *= -1.0f;
            }
            else if (CurrentDistance <= 0.0f)
            {
                CurrentDistance = 0.0f;
                DirectionMultiplier *= -1.0f;
            }
        }
        else
        {
            // Clamp to spline bounds
            CurrentDistance = FMath::Clamp(CurrentDistance, 0.0f, SplineLength);
        }
    }
    
    // Update position along spline
    FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
    SawBaseMesh->SetWorldLocation(NewLocation);
    
    // Update rotation to follow spline direction
    if (bRotateAlongSpline)
    {
        FVector Direction = SplineComponent->GetDirectionAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
        FRotator SplineRotation = Direction.Rotation();
        
        // Add spinning rotation for saw blade effect
        CurrentRotation += RotationSpeed * DeltaTime;
        if (CurrentRotation >= 360.0f)
        {
            CurrentRotation -= 360.0f;
        }
        
        // Combine spline rotation with spinning rotation
        FRotator FinalRotation = SplineRotation;
        FinalRotation.Roll += CurrentRotation;
        
        SawBaseMesh->SetWorldRotation(FinalRotation);
    }
    else
    {
        // Just spin the saw blade without following spline rotation
        SawBladeMesh->AddLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }
}

void ABlob_SawTrap::SetMovementSpeed(float NewSpeed)
{
	MovementSpeed = FMath::Max(0.1f, NewSpeed);
}

void ABlob_SawTrap::ReverseMovementDirection()
{
	DirectionMultiplier *= -1.0f;
}

void ABlob_SawTrap::ResetPosition()
{
	CurrentDistance = 0.0f;
	CurrentRotation = 0.0f;
	DirectionMultiplier = 1.0f;
    
	if (SplineLength > 0.0f)
	{
		FVector InitialLocation = SplineComponent->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
		SawBaseMesh->SetWorldLocation(InitialLocation);
        
		if (bRotateAlongSpline)
		{
			FVector Direction = SplineComponent->GetDirectionAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
			FRotator InitialRotation = Direction.Rotation();
			SawBaseMesh->SetWorldRotation(InitialRotation);
		}
	}
}