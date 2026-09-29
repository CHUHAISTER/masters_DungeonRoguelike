


#include "DungeonRoom.h"


UDungeonRoom::UDungeonRoom(int X, int Y, int Width, int Height) :
	X(X), Y(Y), Width(Width), Height(Height)
{
	Area = Width * Height;
}

Rect UDungeonRoom::ToRect() const
{
	return Rect(X, Y, Width, Height);
}

UDungeonRoom::~UDungeonRoom()
{
}
