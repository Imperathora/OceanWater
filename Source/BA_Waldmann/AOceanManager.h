// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOceanManager.generated.h"

UCLASS()
class BA_WALDMANN_API AAOceanManager : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "FFT")
		FVector RandomGaussNum();

	UFUNCTION(BlueprintCallable, Category = "FFT")
		float PhillipsSpectrum(FVector waveVector, FVector windDirection, float windSpeed, float gravConstant, float constant);

	UFUNCTION(BlueprintCallable, Category = "FFT")
		float GenerateWaveHeight_h0k(FVector windDirection, FVector position, float windSpeed, float gravConstant, float constant);

	UFUNCTION(BlueprintCallable, Category = "FFT")
		float GenerateWaveHeight_h0minusk(FVector windDirection, FVector position, float windSpeed, float gravConstant, float constant);

	UFUNCTION(BlueprintCallable, Category = "FFT")
		float FourierComponents(FVector windDirection, FVector position, float windSpeed, float gravConstant, float constant, float time);

	UFUNCTION(BlueprintCallable, Category = "FFT")
		void FFT(TArray<float> fcInput, TArray<float> fcOutput, int stepSize, int offset);

	UFUNCTION(BlueprintCallable, Category = "Gerstner")
		FVector GenerateGerstnerWave(FVector position, FVector direction, float rotationAngle, float amplitude, float steepness, float speed, float waveDistance, float gravConstant, float time);

	UFUNCTION(BlueprintCallable, Category = "Gerstner")
		FVector GenerateGerstnerWaveSet(FVector position, FVector direction, float rotationAngle, float amplitude, float steepness, float speed, float waveDistance, float gravConstant, float time);

	UFUNCTION(BlueprintCallable, Category = "Gerstner")
		float CalculateWaveLength(float gravConstant, float waveDistance);

	unsigned int reverseBits(unsigned int i, const int fftSize);

	UPROPERTY(BlueprintReadWrite, Category = "FFT", EditAnywhere)
		float k_x;
	UPROPERTY(BlueprintReadWrite, Category = "FFT", EditAnywhere)
		float k_z;


	const int fftSize = 8;

};
