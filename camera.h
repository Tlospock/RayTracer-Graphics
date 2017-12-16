#pragma once
#include "object.h"
#include "triple.h"

class Camera :
	public Object
{
public:
	Camera(Triple eye, Triple center, Triple up, int width, int height);
    Triple eye;
	Triple center;
	Triple up;
	int width;
	int height;

private:
    std::string name;
};

