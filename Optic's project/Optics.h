#pragma once
#include<array>

class Lense;
 
class Source;

class Ray
{
public:
	void send();//отправить луч на отрисовку (как только заполняется end)

	auto coordinates();// вывод координат для расчета аберраций, не уверенна в типе


	//private:
	std::array<int>[3] begin; //начало луча
	std::array<int>[3] end; //конец луча
	std::array<int>[3] cos// направляющие косинусы
};

class Screen;

class Calculate;