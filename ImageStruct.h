#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct sampleDeepRGBA
{
	float deepFront;
	float deepBack;
	float r;
	float g;
	float b;
	float a;

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

struct pixelDeepRGBA
{
	vector<sampleDeepRGBA> pixel;
	pixelDeepRGBA(int sampleCount)
	{
		pixel.resize(sampleCount);
	}
};
class imagePlane
{
public:
	void addLine(const vector<pixelDeepRGBA>& line)
	{
		//m_points.push_back(p);
	}
	void toGPU() {}
	void backToCPU() {}
	void setImageSize(int w, int h)
	{

	}
private:
	vector< vector<pixelDeepRGBA> > m_points;
	vector< vector<int> > imageIndexes;//start of each pixel in m_points
};