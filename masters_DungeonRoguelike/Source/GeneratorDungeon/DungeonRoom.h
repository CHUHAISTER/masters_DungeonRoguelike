

#pragma once

#include "CoreMinimal.h"
#include "DungeonRoom.generated.h"


UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Enemy      UMETA(DisplayName = "Enemy"),
	Loot       UMETA(DisplayName = "Loot"),
	Start      UMETA(DisplayName = "Start"),
	Next	   UMETA(DisplayName = "NextLevel")
};

struct Rect {
	int x, y, w, h;
};

/**
 * 
 */

class GENERATORDUNGEON_API UDungeonRoom
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomType RoomType = ERoomType::Enemy;

	UPROPERTY(VisibleAnywhere)
	int32 Area;

	Rect ToRect() const;

public:
	UDungeonRoom(int X, int Y, int Width, int Height);
	~UDungeonRoom();

};
