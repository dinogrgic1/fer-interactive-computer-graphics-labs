// Local Headers

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>

std::vector<std::pair<int, int>> klikovi;
std::vector<std::pair<int, int>> pravac;

bool secondPress = true;
bool odsijecanje = false;

bool onFrame(int width, int height, int x, int y)
{
	int x0 = height / 4; 
	int y0 = width / 4;

	int x1 = height - x0;
	int y1 = height - y0;
	
	return (x == x0 || x == x1) && (y >= y0 && y <= y1) || 
			(y == y0 || y == y1) && (x >= x0 && x <= x1);
}

bool inFrame(int width, int height, int x, int y)
{
	int x0 = height / 4;
	int y0 = width / 4;

	int x1 = height - x0;
	int y1 = height - y0;
	
	return (x > x0 && x < x1) && (y > y0 - 1 && y < y1 - 1);
}

int color(int x, int y)
{
	const int MAX_LENGTH = 0;
	return 0;
}

// xor swap
void swap(int &x, int &y)
{
	x = x ^ y;
	y = y ^ x;
	x = x ^ y;
}

void bresenhamTo90(int x0, int y0, int x1, int y1)
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
		swap(x0, y0);
		swap(x1, y1);
		
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

void bresenhamFrom90(int x0, int y0, int x1, int y1)
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
		swap(x0, y1);
		swap(x1, y0);

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

void iscrtajLiniju(int x0, int y0, int x1, int y1)
{
	if(x0 <= x1)
		if(y0 <= y1)
			bresenhamTo90(x0, y0, x1, y1);
		else
			bresenhamFrom90(x0, y0, x1, y1);
	else
		if(y0 >= y1)
			bresenhamTo90(x1, y1, x0, y0);
		else
			bresenhamFrom90(x1, y1, x0, y0);
}

void klikMisa(int x, int y, int vrsta)
{
	if (vrsta == 0)
	{
		secondPress = !secondPress;
		klikovi.push_back(std::make_pair(x, y));

		int size = klikovi.size();
		if(secondPress)
		{
			std::pair<int,int> t1 = klikovi[size-2];
			std::pair<int,int> t2 = klikovi[size-1];
			iscrtajLiniju(t1.first, t1.second, t2.first, t2.second);
		}
	}
		
	if (vrsta == 1)
	{
		std::cout << "odsijecanje: " << odsijecanje << std::endl;
		odsijecanje = !odsijecanje;
		klikovi.clear();
		pravac.clear();
	}
}

int main(int argc, char *argv[])
{
	int width = 97, height = 97;
	
	int widhtQuarter = width / 4; 
	int heightQuarter = height / 4; 

	std::cout << argv[0] << std::endl;
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti())
	{
		grafika.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1)
			{
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
				if(odsijecanje && onFrame(width, height, i, j))
					grafika.osvijetliFragment(i, j, glm::vec3(0.18, 0.54, 0.34));
			}

		//iscrtavanje pritisnutih fragmenata
		//ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
		for (int i = 0; i < klikovi.size(); i++)
		{
			grafika.osvijetliFragment(klikovi[i].first, height - klikovi[i].second - 1, glm::vec3(0.6, 0.2, 0));	
		}		

		for (int i = 0; i < pravac.size(); i++)
		{
			if(odsijecanje)
			{
				if(inFrame(width, height, pravac[i].first, pravac[i].second))
					grafika.osvijetliFragment(pravac[i].first, height - pravac[i].second - 1, glm::vec3(0.6, 0.2, 0));
			}
			else
			{
				grafika.osvijetliFragment(pravac[i].first, height - pravac[i].second - 1, glm::vec3(0.6, 0.2, 0));
			}
		}
		grafika.iscrtajRaster();

		//kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		//kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));
	}
	return EXIT_SUCCESS;
}
