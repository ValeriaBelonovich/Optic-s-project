#include "Optics.h"
#include <utility>
>
class Ray
{
public:
	void send()//отправить луч на отрисовку (как только заполняется end)
	{}

	auto equation()//вывод уравнения прямой которой задается луч, не уверенна в типе
	{}

	auto coordinates()// вывод координат для расчета аберраций, не уверенна в типе
	{}

private:
	std::pair<double, double> begin; //начало луча
	std::pair<double, double> end; //конец луча
	double a;
	double b;// направляющие косинусы
};