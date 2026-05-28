#pragma once
#include "GameObject.hpp"
#include "IPhysical.hpp"

class MovingObject : public GameObject, public IPhysics {
	protected:
		float vertSpeed;
		float horizonSpeed;
	public:
		MovingObject(float xPos, float yPos, float oWidth, float oHeight, char type);
		
		float getVertSpeed() const;
		float getHorizonSpeed() const;
		
		void setVertSpeed( float newVertSpeed );
		void setHorizonSpeed( float newHorizonSpeed );
		
		 virtual void updatePhysics(Map& map, GameObject** allObjects, int count);
		virtual void update();
};