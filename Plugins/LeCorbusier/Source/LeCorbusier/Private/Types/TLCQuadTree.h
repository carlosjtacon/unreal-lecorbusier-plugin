// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ULCAsset.h"

class TLCParticle
{
public:
	TLCParticle(FVector2D center, float radius) :
		Center(center),
		Radius(radius)
	{}

	FVector2D Center;
	float Radius;
	ULCAsset* Item;

	bool Contains(FVector2D Point);
	bool Intersects(FBox2D Range);

	FString ToString();
};

class TLCQuadTree
{
public:
	TLCQuadTree(FBox2D boundary, int capacity) :
		Boundary(boundary),
		Capacity(capacity),
		bDivided(false)
	{}

	FBox2D Boundary;
	int Capacity;

	bool Insert(TLCParticle Particle);
	void Query(FBox2D Range, TArray<TLCParticle>& FoundParticles);
	void Query(TLCParticle Range, TArray<TLCParticle>& FoundParticles);

	// ~TLCQuadTree();

private:
	enum QuadNames
	{
		TopLeft = 0,
		TopRight = 1,
		BottomLeft = 2,
		BottomRight = 3
	};

	bool bDivided;
	TLCQuadTree* SubTrees[4];
	TArray<TLCParticle> Particles;

	void Subdivide();
};
