#pragma once


class GameEntity {
public:
	virtual void Update(float dt) = 0;
	float horizontalPosition;
	float verticalPosition;
	int direction; //left = -1, right = 1;

	const int static LEFT = -1;
	const int static RIGHT = 1;
private:
	
};