#pragma once 
#pragma warning (disable : 4244 4800)
#include "DXUT.h"
#if defined(DEBUG) | defined(_DEBUG)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define for_iter(iter, col) for(auto iter = col.begin(); iter != col.end(); ++iter)

#define Vec2(x,y) D3DXVECTOR2((x),(y))
typedef D3DXVECTOR2 Vec2;

using namespace std;
using namespace std::tr2::sys;

class Rect : public RECT
{
public:
	Rect()
	{
		top = bottom = left = right = 0;
	}
	Rect(int l, int r, int t, int b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}

	int width()
	{
		return right - left;
	}

	int height()
	{
		return bottom - top;
	}
	Vec2 center()
	{
		return Vec2(width(), height()) / 2;
	}
	Rect offset(Vec2 pos)
	{
		Rect t = *this;
		t.left += pos.x;
		t.right += pos.x;
		t.top += pos.y;
		t.bottom += pos.y;
		return t;
	}
	bool intersects(Rect t, Rect *inter)
	{
		return IntersectRect(inter, this, &t);
	}

	bool contains(Vec2 p)
	{
		return p.x > left && p.x < right && p.y > top && p.y < bottom;
	}

	void curOff(int cut)
	{
		top += cut;
		bottom -= cut;
		left += cut;
		right -= cut;
	}
};

inline float calAngle(Vec2 p1, Vec2 p2)
{
	return atan2(p2.y - p1.y, p2.x - p1.x);
}

inline int random(int f, int t)
{
	static default_random_engine random(timeGetTime());
	uniform_int_distribution<int> d(f, t);
	return d(random);
}

inline float random(float f, float t)
{
	static default_random_engine random(timeGetTime());
	uniform_real_distribution<float> d(f, t);
	return d(random);
}