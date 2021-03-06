#pragma once
#ifndef HILBERT_H
#define HILBERT_H
#include "Pch.h"

struct Point
{
	int x;
	int y;
};

struct Hilbert
{
private:
	static void rot(int n, int* x, int* y, int rx, int ry)
	{
		if (ry == 0)
		{
			if (rx == 1)
			{
				*x = n - 1 - *x;
				*y = n - 1 - *y;
			}

			//Swap x and y
			int t = *x;
			*x = *y;
			*y = t;
		}
	}

	//XY坐标到Hilbert代码转换
	static int xy2d(int n, int x, int y)
	{
		int rx, ry, s, d = 0;
		for (s = n / 2; s > 0; s /= 2)
		{
			rx = (x & s) > 0;
			ry = (y & s) > 0;
			d += s * s * ((3 * rx) ^ ry);
			rot(s, &x, &y, rx, ry);
		}
		return d;
	}

	//Hilbert代码到XY坐标
	static void d2xy(int n, int d, int* x, int* y)
	{
		int rx, ry, s, t = d;
		*x = *y = 0;
		for (s = 1; s < n; s *= 2)
		{
			rx = 1 & (t / 2);
			ry = 1 & (t ^ rx);
			rot(s, x, y, rx, ry);
			*x += s * rx;
			*y += s * ry;
			t /= 4;
		}
	}

public:
	static void Generate(int n, Point* pts)
	{
		for (int i = 0; i < n * n; i++)
		{
			d2xy(n, i, &pts[i].x, &pts[i].y);
		}
	}
};


#endif //HILBERT_H
