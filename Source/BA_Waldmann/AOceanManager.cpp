// Fill out your copyright notice in the Description page of Project Settings.


#include "AOceanManager.h"
#include <complex>
#include <iostream>
#include <Runtime\AssetRegistry\Public\AssetRegistry\AssetRegistryModule.h>

AAOceanManager::AAOceanManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

FVector AAOceanManager::RandomGaussNum()
{
	int64 DateInSeconds = FDateTime::Now().ToUnixTimestamp();
	FRandomStream SRand = FRandomStream();
	SRand.Initialize(DateInSeconds);

	float m_random_1 = SRand.FRandRange(0.0f, 1.0f);
	float m_random_2 = SRand.FRandRange(0.0f, 1.0f);

	if (m_random_1 < 1e-6f)
		m_random_1 = 1e-6f;

	float m_Gauss_1 = sqrt(-2 * FMath::Log2(m_random_1)) * cos(2 * PI * m_random_2);
	float m_Gauss_2 = sqrt(-2 * FMath::Log2(m_random_1)) * sin(2 * PI * m_random_2);


	FVector vect;
	vect.X = m_Gauss_1;
	vect.Y = m_Gauss_2;
	vect.Z = 0;

	return(vect);
}

float AAOceanManager::PhillipsSpectrum(FVector waveVector, FVector windDirection, float windSpeed, float gravConstant, float constant)
{
	float biggestWave = FMath::Pow(windSpeed, 2.0f) / gravConstant;
	float biggestWave_2 = biggestWave * biggestWave;

	float waveVectorLength = sqrt(FMath::Pow(waveVector.X, 2.f) + FMath::Pow(waveVector.Y, 2.f) + FMath::Pow(waveVector.Z, 2.f));

	if (waveVectorLength < 0.00001f)
		return 0;

	float waveVector_2 = waveVectorLength * waveVectorLength;
	float waveVector_4 = waveVector_2 * waveVector_2;

	float windDirLength = sqrt(FMath::Pow(windDirection.X, 2.f) + FMath::Pow(windDirection.Y, 2.f) + FMath::Pow(windDirection.Z, 2.f));

	float waveDotWind = Dot3(waveVector, windDirection);
	float waveDotWind_2 = waveDotWind * waveDotWind;

	float spectrum = constant * (FMath::Exp(-1.0f / (waveVector_2 * biggestWave_2)) / waveVector_4 * waveDotWind_2);

	return spectrum;
}

float AAOceanManager::GenerateWaveHeight_h0k(FVector windDirection, FVector position, float windSpeed, float gravConstant, float constant)
{
	float biggestWave = FMath::Pow(windSpeed, 2.0f) / gravConstant;

	k_x = 2 * PI * position.X / biggestWave;

	k_z = 2 * PI * position.Y / biggestWave;

	FVector k;
	k.X = k_x;
	k.Z = k_z;

	FVector m_Gauss = RandomGaussNum();
	float m_Gauss_1 = m_Gauss.X;
	float m_Gauss_2 = m_Gauss.Y;

	float h0k = 1.0f / sqrt(2) * (m_Gauss_1 + m_Gauss_2 * sqrt(PhillipsSpectrum(k, windDirection, windSpeed, gravConstant, constant)));

	return h0k;
}

float AAOceanManager::GenerateWaveHeight_h0minusk(FVector windDirection, FVector position, float windSpeed, float gravConstant, float constant)
{
	float biggestWave = FMath::Pow(windSpeed, 2.0f) / gravConstant;

	k_x = 2 * PI * position.X / biggestWave;

	k_z = 2 * PI * position.Y / biggestWave;


	FVector k;
	k.X = k_x;
	k.Z = k_z;
	k = k * -1;

	FVector m_Gauss = RandomGaussNum();
	float m_Gauss_1 = m_Gauss.X;
	float m_Gauss_2 = m_Gauss.Y;

	float h0_k = 1.0f / sqrt(2) * (m_Gauss_1 + m_Gauss_2 * sqrt(PhillipsSpectrum(k, windDirection, windSpeed, gravConstant, constant)));

	return h0_k;
}

float AAOceanManager::FourierComponents(FVector windDirection, FVector position, float windSpeed, float gravConstant, float constant, float time)
{
	float biggestWave = FMath::Pow(windSpeed, 2.0f) / gravConstant;

	k_x = 2 * PI * position.X / biggestWave;
	k_z = 2 * PI * position.Y / biggestWave;

	float lengthK = sqrt(FMath::Pow(k_x, 2.f) + FMath::Pow(k_z, 2.f));
	float windDirLength = sqrt(FMath::Pow(windDirection.X, 2.f) + FMath::Pow(windDirection.Y, 2.f) + FMath::Pow(windDirection.Z, 2.f));

	float w = sqrt(gravConstant * lengthK);

	float fourierComponent = GenerateWaveHeight_h0k(windDirection, position, windSpeed, gravConstant, constant) * FMath::Exp(windDirLength * time) + GenerateWaveHeight_h0minusk(windDirection, position, windSpeed, gravConstant, constant) * FMath::Exp(-1 * windDirLength * time);

	return fourierComponent;
}

void AAOceanManager::FFT(TArray<float> fcInput, TArray<float> fcOutput, int stepSize, int offset)
{

	TArray<float> x[8];


	for (int i = 0; i < fftSize; i++)
	{
		x->Insert(fcInput, (i));
	}

	TArray<float> fftArray;

	int numStages;
	numStages = log(fftSize) / log(2);

	int firstButterfly;
	int lastButterfly;
	firstButterfly = 1;
	lastButterfly = fftSize / 2;


	for (int j = 0; j < fftSize; j++)
	{
		unsigned int y = reverseBits(j, fftSize) >> (32 - numStages) & (fftSize - 1);
		fftArray.Insert(x[j], y);

	}


	for (int i = 0; i < numStages; i++) {

		int butterflyGroupHeight = 1 << i;
		int butterflyGroupHalfHeight = butterflyGroupHeight >> 1;

		std::complex<float> twiddleFactor(cos(2 * PI / butterflyGroupHeight), sin(2 * PI / butterflyGroupHeight));


		for (int k = 1; k <= fftSize; k += butterflyGroupHeight)
		{
			std::complex<float> initTwiddleFactor(0, 1);

			unsigned int iPhase = i & stepSize - offset;


			for (int n = 0; n < butterflyGroupHalfHeight; n++)
			{
				float y = fftArray[k + n];
				float z = initTwiddleFactor.real() * fftArray[k + n + butterflyGroupHalfHeight];

				fftArray[k + n] = y + z;
				fftArray[k + n + butterflyGroupHalfHeight] = y - z;

				initTwiddleFactor = initTwiddleFactor * twiddleFactor;

				fcOutput[k] = fftArray[k];


			}


		}
	}


}

unsigned int AAOceanManager::reverseBits(unsigned int i, const int size)
{
	unsigned int reverse = 0;

	reverse = (reverse << 1) + (i & 1);
	i >>= 1;

	return reverse;
}

