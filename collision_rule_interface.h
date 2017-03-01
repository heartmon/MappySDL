#pragma once
#include <algorithm>

class Message;
class GameEntity;
class CollisionRuleInterface {
public:
	virtual void Create(GameEntity* gameEntity) {
		this->gameEntity = gameEntity;
	};
	virtual void didHit(Message* message) {};
	virtual int isCollided(GameEntity* self, GameEntity* withThisEntity, float dt) { return -1; }
	virtual int isCollidedWithMap(GameEntity* withThisEntity, float dt, std::vector<Tile*>* tileMap) {
		//auto -> use default of component
		return -1; 
		// return 0 ; false
		// return 1 ; true
	};

	bool checkSquareCollision(GameEntity::Box a, GameEntity::Box b) {
		// The sides of the rectangles
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of rect A
		leftA = a.x;
		rightA = a.x + a.w;
		topA = a.y;
		bottomA = a.y + a.h;

		//Calculate the sides of rect B
		leftB = b.x;
		rightB = b.x + b.w;
		topB = b.y;
		bottomB = b.y + b.h;

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
	}

	virtual float SweptAABB(GameEntity::Box movingBox, GameEntity::Box staticBox, float &normalX, float &normalY) {
		float xInvEntry, yInvEntry;
		float xInvExit, yInvExit;

		// find the distance between the objects on the near and far sides for both x and y
		if (movingBox.vx > 0.0f)
		{
			xInvEntry = staticBox.x - (movingBox.x + movingBox.w);
			xInvExit = (staticBox.x + staticBox.w) - movingBox.x;
		}
		else
		{
			xInvEntry = (staticBox.x + staticBox.w) - movingBox.x;
			xInvExit = staticBox.x - (movingBox.x + movingBox.w);
		}

		if (movingBox.vy > 0.0f)
		{
			yInvEntry = staticBox.y - (movingBox.y + movingBox.h);
			yInvExit = (staticBox.y + staticBox.h) - movingBox.y;
		}
		else
		{
			yInvEntry = (staticBox.y + staticBox.h) - movingBox.y;
			yInvExit = staticBox.y - (movingBox.y + movingBox.h);
		}

		// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
		float xEntry, yEntry;
		float xExit, yExit;

		if (movingBox.vx == 0.0f)
		{
			xEntry = -std::numeric_limits<float>::infinity();
			xExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			xEntry = xInvEntry / movingBox.vx;
			xExit = xInvExit / movingBox.vx;
		}

		if (movingBox.vy == 0.0f)
		{
			yEntry = -std::numeric_limits<float>::infinity();
			yExit = std::numeric_limits<float>::infinity();
		}
		else
		{
			yEntry = yInvEntry / movingBox.vy;
			yExit = yInvExit / movingBox.vy;
		}

		// find the earliest/latest times of collision
		float entryTime = std::max(xEntry, yEntry);
		float exitTime = std::min(xExit, yExit);

		// if there was no collision
		if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
		{
			normalX = 0.0f;
			normalY = 0.0f;
			return 1.0f;
		}
		else // if there was a collision
		{
			// calculate normal of collided surface
			if (xEntry > yEntry)
			{
				if (xInvEntry < 0.0f)
				{
					normalX = 1.0f;
					normalY = 0.0f;
				}
				else
				{
					normalX = -1.0f;
					normalY = 0.0f;
				}
			}
			else
			{
				if (yInvEntry < 0.0f)
				{
					normalX = 0.0f;
					normalY = 1.0f;
				}
				else
				{
					normalX = 0.0f;
					normalY = -1.0f;
				}
			}
		}

		// return the time of collision
		return entryTime;

	}
protected:
	GameEntity* gameEntity;
private:
};