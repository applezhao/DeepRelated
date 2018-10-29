#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct pointXYDeepRGBA
{
	float x;
	float y;
	float deepFront;
	float deepBack;
	float r;
	float g;
	float b;
	float a;

	void set2dCoord(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	void setDeep(float front, float back)
	{
		deepFront = front;
		deepBack = back;
	}
	void setColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};
class pointCloud
{
public:
	void addPoint(const pointXYDeepRGBA& p)
	{
		m_points.push_back(p);
	}
	void toGPU() {}
	void backToCPU() {}
	void setImageSize(int w, int h)
	{

	}
private:
	vector<pointXYDeepRGBA> m_points;
	vector< vector<int> > imageIndexes;//start of each pixel in m_points
};

