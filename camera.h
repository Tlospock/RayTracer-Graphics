#pragma once
#include "object.h"
#include "triple.h"

class Camera :
	public Object
{
public:
	Camera(Triple eye, Triple center, Triple up, Vector viewSize);

private:
	Triple eye;
	Triple center;
	Triple up;
	Vector viewSize;
};

