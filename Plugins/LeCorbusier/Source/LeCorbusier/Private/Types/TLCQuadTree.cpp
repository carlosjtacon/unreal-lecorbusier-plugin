// Fill out your copyright notice in the Description page of Project Settings.

#include "TLCQuadTree.h"


/** TLCParticle */
bool TLCParticle::Contains(FVector2D Point)
{
	return FMath::Square(Point.X - Center.X) + FMath::Square(Point.Y - Center.Y) < FMath::Square(Radius);
}

bool TLCParticle::Intersects(FBox2D Range)
{
	float XDist = FMath::Abs(Range.GetCenter().X - Center.X);
	float YDist = FMath::Abs(Range.GetCenter().Y - Center.Y);

	float Width = Range.GetExtent().X;
	float Height = Range.GetExtent().Y;

	float Edges = FMath::Square(XDist - Width) + FMath::Square(YDist - Height);

	// No intersection
	if (XDist > (Radius + Width) || YDist > (Radius + Height)) return false;

	// Intersection within the circle
	if (XDist <= Width || YDist <= Height) return true;

	// Intersection on the edge of the circle
	return Edges <= FMath::Square(Radius);
}

FString TLCParticle::ToString()
{
	return FString(TEXT("TLCParticle - Center: ")) + Center.ToString() 
		+ FString(TEXT(", Radius: ")) + FString::SanitizeFloat(Radius);
}


/** TLCQuadTree */
bool TLCQuadTree::Insert(TLCParticle Particle)
{
    // Cuidado con este método, igual toca reescribirlo con los teniendo en cuenta el límite del box
    // UE_LOG(LogTemp, Warning, TEXT("!Boundary.IsInside(Particle.Center)=%s"), (!Boundary.IsInside(Particle.Center) ? TEXT("True") : TEXT("False")));
    // UE_LOG(LogTemp, Warning, TEXT("Boundary=%s"), *Boundary.ToString());
    // UE_LOG(LogTemp, Warning, TEXT("Particle.Center=%s"), *Particle.Center.ToString());
    if (!Boundary.IsInside(Particle.Center)) return false;

    if (Particles.Num() < Capacity)
    {
        Particles.Add(Particle);
        // UE_LOG(LogTemp, Error, TEXT("Inserted In QuadTree"));
        // UE_LOG(LogTemp, Warning, TEXT("Inserted Particle at %s"), *Particle.Center.ToString());
        return true;
    }
    
    if (!bDivided)
    {
        Subdivide();
    }

    return SubTrees[TopLeft]->Insert(Particle) || SubTrees[TopRight]->Insert(Particle)
        || SubTrees[BottomLeft]->Insert(Particle) || SubTrees[BottomRight]->Insert(Particle);
}

void TLCQuadTree::Subdivide()
{
	// UE_LOG(LogTemp, Log, TEXT("Subdivide QuadTree"));

    /**
	 *  ___________max
	 * |     |     |
	 * |     |     |
	 * |-----c------
	 * |     |     |
	 * min___|_____|
	 *
	 */

    FVector2D Center = Boundary.GetCenter();

    FBox2D TL = FBox2D(FVector2D(Boundary.Min.X, Center.Y), FVector2D(Center.X, Boundary.Max.Y));
    FBox2D TR = FBox2D(Center, Boundary.Max);
    FBox2D BL = FBox2D(Boundary.Min, Center);
    FBox2D BR = FBox2D(FVector2D(Center.X, Boundary.Min.Y), FVector2D(Boundary.Max.X, Center.Y));

    SubTrees[TopLeft] = new TLCQuadTree(TL, Capacity);
    SubTrees[TopRight] = new TLCQuadTree(TR, Capacity);
    SubTrees[BottomLeft] = new TLCQuadTree(BL, Capacity);
    SubTrees[BottomRight] = new TLCQuadTree(BR, Capacity);

    bDivided = true;
}

void TLCQuadTree::Query(FBox2D Range, TArray<TLCParticle>& FoundParticles)
{
    // UE_LOG(LogTemp, Error, TEXT("Query QuadTree"));
    // UE_LOG(LogTemp, Warning, TEXT("!Boundary.Intersect(Range)=%s"), (!Boundary.Intersect(Range) ? TEXT("True") : TEXT("False")));
    // UE_LOG(LogTemp, Warning, TEXT("Boundary=%s"), *Boundary.ToString());
    // UE_LOG(LogTemp, Warning, TEXT("Range=%s"), *Range.ToString());

    if (!Boundary.Intersect(Range)) return;

	for(int i = 0; i < Particles.Num(); i++)
    {
        // UE_LOG(LogTemp, Warning, TEXT("Range.IsInside(Particles[i].Center)=%s"), (Range.IsInside(Particles[i].Center) ? TEXT("True") : TEXT("False")));
        // UE_LOG(LogTemp, Warning, TEXT("Range=%s"), *Range.ToString());
        // UE_LOG(LogTemp, Warning, TEXT("Particles[i].Center=%s"), *Particles[i].Center.ToString());
        if (Range.IsInside(Particles[i].Center))
        {
            FoundParticles.Add(Particles[i]);
            // UE_LOG(LogTemp, Error, TEXT("Added to FoundParticles"));
        }
    }
    
    if (bDivided)
    {
        SubTrees[TopLeft]->Query(Range, FoundParticles);
        SubTrees[TopRight]->Query(Range, FoundParticles);
        SubTrees[BottomLeft]->Query(Range, FoundParticles);
        SubTrees[BottomRight]->Query(Range, FoundParticles);
    }
}

void TLCQuadTree::Query(TLCParticle Range, TArray<TLCParticle>& FoundParticles)
{
	// UE_LOG(LogTemp, Error, TEXT("Query QuadTree"));
	// UE_LOG(LogTemp, Warning, TEXT("!Boundary.Intersect(Range)=%s"), (!Boundary.Intersect(Range) ? TEXT("True") : TEXT("False")));
	// UE_LOG(LogTemp, Warning, TEXT("Boundary=%s"), *Boundary.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Range=%s"), *Range.ToString());

	if (!Range.Intersects(Boundary)) return;

	for (int i = 0; i < Particles.Num(); i++)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Range.IsInside(Particles[i].Center)=%s"), (Range.IsInside(Particles[i].Center) ? TEXT("True") : TEXT("False")));
		// UE_LOG(LogTemp, Warning, TEXT("Range=%s"), *Range.ToString());
		// UE_LOG(LogTemp, Warning, TEXT("Particles[i].Center=%s"), *Particles[i].Center.ToString());
		if (Range.Contains(Particles[i].Center))
		{
			FoundParticles.Add(Particles[i]);
			// UE_LOG(LogTemp, Error, TEXT("Added to FoundParticles"));
		}
	}

	if (bDivided)
	{
		SubTrees[TopLeft]->Query(Range, FoundParticles);
		SubTrees[TopRight]->Query(Range, FoundParticles);
		SubTrees[BottomLeft]->Query(Range, FoundParticles);
		SubTrees[BottomRight]->Query(Range, FoundParticles);
	}
}

// TLCQuadTree::~TLCQuadTree()
// {
//     for (TLCQuadTree* SubTree : SubTrees)
// 	{
// 		delete SubTree;
// 		SubTree = nullptr;
// 	}
// }
