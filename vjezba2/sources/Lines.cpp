#include "Lines.h"

#include <utility>
#include <vector>

void Lines::bresenhamTo90(std::vector<std::pair<int, int>> &pravac, int x0, int y0, int x1, int y1)
{
	int yc, correction;
	int a, yf;

	if(y1 - y0 <= x1 - x0)
	{
		a = 2 * (y1 - y0);
		yc = y0;
		yf = -(x1 - x0);
		correction = -2 * (x1 - x0);

		for (int x = x0; x <= x1; x++)
		{
			pravac.push_back(std::make_pair(x, yc));
			yf += a;
			if (yf > 0)
			{
				yf += correction;
				yc++;
			}
		}
	} 
	else 
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		
		a = 2 * (y1 - y0);
		yc = y0;
		yf = -(x1 - x0);
		correction = -2 * (x1 - x0);

		for (int x = x0; x <= x1; x++)
		{
			pravac.push_back(std::make_pair(yc, x));
			yf += a;
			if (yf > 0)
			{
				yf += correction;
				yc++;
			}
		}
	}
}

void Lines::bresenhamFrom90(std::vector<std::pair<int, int>> &pravac, int x0, int y0, int x1, int y1)
{
	int yc, correction;
	int a, yf;

	if(-(y1 - y0) <= x1 - x0)
	{
		a = 2 * (y1 - y0);
		yc = y0;
		yf = x1 - x0;
		correction = 2 * (x1 - x0);

		for (int x = x0; x <= x1; x++)
		{
			pravac.push_back(std::make_pair(x, yc));
			yf += a;
			if (yf <= 0)
			{
				yf += correction;
				yc--;
			}
		}
	} 
	else 
	{
		std::swap(x0, y1);
		std::swap(x1, y0);

		a = 2 * (y1 - y0);
		yc = y0;
		yf = x1 - x0;
		correction = 2 * (x1 - x0);

		for (int x = x0; x <= x1; x++)
		{
			pravac.push_back(std::make_pair(yc, x));
			yf += a;
			if (yf <= 0)
			{
				yf += correction;
				yc--;
			}
		}
	}
}

void Lines::drawLine(std::vector<std::pair<int, int>> &pravac, int x0, int y0, int x1, int y1)
{
	if(x0 <= x1)
		if(y0 <= y1)
			bresenhamTo90(pravac, x0, y0, x1, y1);
		else
			bresenhamFrom90(pravac, x0, y0, x1, y1);
	else
		if(y0 >= y1)
			bresenhamTo90(pravac, x1, y1, x0, y0);
		else
			bresenhamFrom90(pravac, x1, y1, x0, y0);
}