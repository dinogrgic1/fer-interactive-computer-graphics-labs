// Local Headers
#include "Polygon.h"
#include "Lines.h"

// System Headers
#include "Grafika.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>

#include <iostream>

bool filled = false;

std::vector<std::pair<int, int>> klikovi;
std::vector<std::pair<int, int>> inside;
std::vector<std::pair<int, int>> pravac;
Polygon polygon = Polygon();

bool cw = false;

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

// xor swap
void swap(int &x, int &y)
{
	x = x ^ y;
	y = y ^ x;
	x = x ^ y;
}

void klikMisa(int x, int y, int vrsta)
{
	if (vrsta == 0)
	{
		if(!filled)
		{
			klikovi.push_back(std::make_pair(x, y));
			polygon.addVertex(x,y);

			int size = klikovi.size();
			if(size > 1)
			{
				std::pair<int,int> t1 = klikovi[size-2];
				std::pair<int,int> t2 = klikovi[size-1];
				if(!polygon.isConvex())
				{
					std::cout << "[Error] Poligon nije konkavan..." << std::endl;
					polygon.popVertex();
					klikovi.pop_back();
				}
				else
				{
					Lines().drawLine(pravac, t1.first, t1.second, t2.first, t2.second);
				}
			}
		}
		else
		{
			if(polygon.dotIsInside(x, y))
			{
				inside.push_back(std::make_pair(x, y));
			}
			else
			{
				klikovi.push_back(std::make_pair(x, y));
			}
			
		}
	}
	
	if (vrsta == 1)
	{
		polygon.setClockwise();
		polygon.draw(pravac);
		filled = true;
	}
}
int main(int argc, char * argv[]) {

	int width = 97, height = 97;
	std::cout << argv[0] << std::endl;
	Grafika grafika(width, height, glm::vec3(0, 0, 0), argv[0]);

	//prosljeduje se pokazivac na funkciju. Ta funkcija ce se pozvati kada korisnik pritisne misem
	grafika.registrirajFunkcijuZaKlikMisa(klikMisa);

	while (grafika.trebaZatvoriti()) {

		//osvjezavanje pozicije razlicitih objekata i fizikalna simulacija bi se izvrsavala u ovom dijelu

		grafika.pobrisiProzor();

		//iscrtavanje uzorka sahovnice
		for (int i = 0; i < height; i += 1)
			for (int j = 0; j < width; j += 1) {
				if ((i + j) % 2 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
				if (i % 10 == 0 && j % 10 == 0)
					grafika.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
			}

		for (int i = 0; i < klikovi.size(); i++)
		{
			grafika.osvijetliFragment(klikovi[i].first, height - klikovi[i].second - 1, glm::vec3(0.6, 0.2, 0));	
		}		

		for (int i = 0; i < pravac.size(); i++)
		{
			grafika.osvijetliFragment(pravac[i].first, height - pravac[i].second - 1, glm::vec3(0.6, 0.2, 0));
		}

		for (int i = 0; i < inside.size(); i++)
		{
			grafika.osvijetliFragment(inside[i].first, height - inside[i].second - 1, glm::vec3(0.18, 0.54, 0.34));
		}

		grafika.iscrtajRaster();

		//kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv funkcije za cekanje
		//kao npr Sleep ili od c++11 na dalje this_thread::sleep_for(chrono::milliseconds(16));
	}
    return EXIT_SUCCESS;
}
