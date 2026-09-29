// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonRoom.h"
#include "Engine/StaticMeshActor.h"
#include <GameCore/EnemyCharacter.h>
#include "Misc/Optional.h"

#include "DungeonGenerator.generated.h"


UENUM(BlueprintType)
enum class EDungeonGenerationMethod : uint8
{
	BSP                 UMETA(DisplayName = "BSP (Rooms & Corridors)"),
	CellularAutomata    UMETA(DisplayName = "Cellular Automata (Caves)")
};

UCLASS()
class GENERATORDUNGEON_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	struct FDsu {
		TArray<int32> Parent;
		FDsu(int32 n) {
			Parent.Init(0, n);
			for (int32 i = 0; i < n; i++) Parent[i] = i;
		}
		int32 Find(int32 i) {
			while (Parent[i] != i) {
				Parent[i] = Parent[Parent[i]];
				i = Parent[i];
			}
			return i;
		}
		void Union(int32 i, int32 j) {
			int32 rootI = Find(i);
			int32 rootJ = Find(j);
			if (rootI != rootJ) Parent[rootI] = rootJ;
		}
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Settings")
	EDungeonGenerationMethod GenerationMethod = EDungeonGenerationMethod::BSP;


protected:
	virtual void BeginPlay() override;
	void GenerateBSP(TArray<TArray<TCHAR>>& grid);
	void GenerateCaves(TArray<TArray<TCHAR>>& grid);
	void SpawnPlayerForCaves(const TArray<TArray<TCHAR>>& grid);

private:


	

	struct Node {
		Rect rect;
		TUniquePtr<Node> left = nullptr;
		TUniquePtr<Node> right = nullptr;
	};


	TArray<TUniquePtr<UDungeonRoom>> Rooms;

	void initializeGrid(TArray<TArray<TCHAR>>& grid);

	bool splitNode(Node* node, int32 minSizeArea);

	void createRooms(Node* node);

	void splitRecursively(Node* node, int32 minSize, int32 maxIterations);

	void dlaBlur(TArray<TArray<TCHAR>>& grid, int32 iterations);

	void drawRoom(TArray<TArray<TCHAR>>& grid) const;

	void createCorridor(TArray<TArray<TCHAR>>& grid, const Rect& a, const Rect& b, int32 bonusWall);

	void TransformRoomsToWorldCoordinates();
	
	void DeleteUnseenWall(TArray<TArray<TCHAR>>& grid);

	void SelectStartEndRoom();

	void SpawnElement();

	void SpawnEnemy();

	void GenerateNavMesh();

	void DrawDungeonVisualZones(TArray<TArray<TCHAR>>& grid, TArray<TArray<int32>>& Labels, FDsu& Dsu);



public:	
	UPROPERTY(EditAnywhere)
	float TileSize = 100.0f;

	// dungeon size
	UPROPERTY(EditAnywhere)
	int32 DungeonWidth = 150;

	UPROPERTY(EditAnywhere)
	int32 DungeonHeight = 150;

	UPROPERTY(EditAnywhere)
	int32 MinSizeArea = 5;

	UPROPERTY(EditAnywhere)
	int32 MaxIterations = 5;

	UPROPERTY(EditAnywhere)
	int32 RoomMargin = 3;

	UPROPERTY(EditAnywhere)
	int32 BonusWall = 5;

	UPROPERTY(EditAnywhere)
	int32 PowerBlur = 2500;


	// Instanced Static Mesh Component
	UPROPERTY(VisibleAnywhere)
	class UInstancedStaticMeshComponent* WallISM;

	// Instanced Static Mesh Component для підлоги
	UPROPERTY(VisibleAnywhere)
	class UInstancedStaticMeshComponent* FloorISM;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DebugMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStaticMeshActor> FloorActorClass;

	void DrawDungeon(TArray<TArray<TCHAR>>&);

	void GenerateCellularAutomata(TArray<TArray<TCHAR>>& grid, int32 FillPercent, int32 Iterations);
	void CheckConnectivityParallel(TArray<TArray<TCHAR>>& grid);
private:
	TUniquePtr<Node> root;
	int32 GetSurroundingWallCount(int32 GridX, int32 GridY, const TArray<TArray<TCHAR>>& CurrentGrid);
};
