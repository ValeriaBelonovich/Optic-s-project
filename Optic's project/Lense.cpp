#include "Optics.h"
#include <utility>

class Lense
{
public:
	std::pair<double, double> intersection_in(Ray& ray) // пересечение внутрь линзы
	{}

	Ray refraction_in(Ray & ray) // прломление внутрь линзы
	{}

	std::pair<double, double> intersection_out(Ray& ray) // пересечение из линзы
	{}

	Ray refraction_out(Ray & ray) // преломление из линзы
	{}

private:
	std::pair<double,double> centre; //координаты центра
	double length; //длина
	double n; // показатель преломления
	double r_left; //левый радиус кривизны
	double r_rightж // правый радиус кривизны
};