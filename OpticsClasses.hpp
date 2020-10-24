#pragma once

#ifdef OPTICSPROJECT_EXPORTS
#define OPTICSPROJECT_API __declspec(dllexport)
#else
#define OPTICSPROJECT_API __declspec(dllimport)
#endif
#pragma warning(disable : 4996)

#include <boost/math/special_functions.hpp>
#include <boost/asio.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/bind.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/triangular.hpp>


#include "nlohmann/json.hpp"

//#include<algorithm>
//#include <chrono>
//#include <cmath>
//#include <filesystem>
//#include <fstream>
#include<future>
//#include<iostream>
#include<mutex>
#include <random>
#include <shared_mutex>
//#include <thread>
//#include<valarray>
#include<vector>
#include<string>


namespace Opticsproject
{
	const size_t dim = 3; //размерность
	const double eps = 0.00000001;
	using nlohmann::json;
	using dot = boost::numeric::ublas::vector<double>;
	using vector = boost::numeric::ublas::vector<double>;

	extern "C" OPTICSPROJECT_API std::string system_run(std::string system); // внешняя функция от которой пляшем
	//extern "C" OPTICSPROJECT_API size_t r_size();



	class Ray
	{
	public:
		Ray() :
			begin(dim),
			end(dim),
			cos(dim),
			n(0)
		{};
		~Ray() = default;
		void send(json &r);// послать на отрисовку оно вообще надо?

		dot begin; // начало
		dot end; //конец
		vector cos; // направляющие векторы
		double n; // показатель преломения среды в которой луч находится
		std::vector<int> rgb;
	};

	//базовый класс всех объектов
	class Object
	{
	public:
		Object() : centre(dim) {};
		virtual ~Object() {};
		virtual dot intersection(Ray& ray) = 0;//пересечение с объектом
		virtual Ray refraction(Ray& ray) = 0; //{ return ray; }; // преломление (почему возвращает по дефолту тот же луч?
		virtual bool end(Ray& ray) { return false; }; //вообще хз зачем возможно говорит, закончен ли луч

	private:
		dot centre;
	};

	//class Screen : public Object
	//{
	//public:
	//	Screen() : centre(dim) {};
	//	bool end(Ray& ray); //если пересекается - заканчивает луч и отправляет на отрисовку true, если нет то false
	//private:
	//	double length;//длина экрана всегда квадратный???
	//	dot centre;
	//};

	class Source
	{
	public:

		Source(): centre(dim) {};
		Source(json& j);
		~Source() = default;
		std::vector<Ray> produce_ray(double apepture, size_t n); //генерирует вектор лучей с заданной апертурой (подумать как задать апертутуру)

	private:
		dot centre;
		std::vector<int> rgb;
	};

	class Lense : public Object
	{

	public:
		Lense() : centre(dim) {};
		Lense(json& j); //нахрена?

		~Lense() = default;

		Ray refraction(Ray& ray) override; // преломление (выдает луч после выхода из линзы)
		double aperture(dot s);// вычисляет апертуру шта? Лучше апертуру вычислять от источника
		dot intersection(Ray& ray) override;

	private:
		dot intersection(Ray& ray, double r);
		Ray refraction(Ray& ray, double r); // луч 
		vector slu(double alpha, double beta, vector& normal, vector& ray); //решение слу для определения координат преломленного луча

	private:
		dot centre; //координаты центра
		double length; //длина всей! линзы
		double n; // показатель преломления
		double r_left; //левый радиус кривизны больше нуля если линза выгнута влево
		double r_right; // правый радиус кривизны меньше нуля если выгнута вправо
	};

	class System
	{
	public:
		//System() {};
		System(json& j);
		~System() = default;
		json get_result()
		{
			return r;
		}
		void run();
	private:
		void trace();
		double saperture();//возможно уберу
		bool boarders(Ray& ray); // 
	private:
		std::vector<Source> sources;
		std::vector<std::unique_ptr<Object>> system;
		std::vector<Ray> rays;
		std::mutex m_rays;
		std::shared_mutex m_system;
		std::mutex m_send;
		json r;
		double dx;
		double dy;
		double dz;
	};

	

}