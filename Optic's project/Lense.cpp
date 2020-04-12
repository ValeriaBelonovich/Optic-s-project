#include "Optics.h"
#include "Ray.cpp"
#include <valarray>
#include <cmath>
#include <boost/math/special_functions/sign.hpp> //обожаю прогу

class Lense
{
public:
	
	Ray refraction_in(Ray & ray) // прломление внутрь линзы
	{}

	Ray refraction_out(Ray & ray) // преломление из линзы
	{}

private:
	std::valarray<double> intersection(Ray& ray, double r);//ищем пересечние луча со сферической поверхностью заданного радиуса кривизны
	std::valarray<double> refraction(Ray& ray, double r);
	std::valarray<double> slu();

private:
	std::valarray<double> centre; //координаты центра
	double length; //длина
	double n; // показатель преломления
	double r_left; //левый радиус кривизны
	double r_right // правый радиус кривизны
};

std::valarray<double> Lense::intersection(Ray& ray, double r) //ищем пересечние луча со сферической поверхностью заданного радиуса кривизны
{
	std::valarray<double> result;
	double centre_curve = centre[0] + boost::math::sign(r)*sqrt(pow(r, 2) + pow(length / 2, 2)); //центр  кривизны поверхности
	double a = pow(ray.x, 2) + pow(ray.y, 2) + pow(ray.z, 2); // коэффициенты в получающемся квадратном уравнении
	double b = 2 * (ray.x * (ray.begin[0] - centre_curve) + ray.y * ray.begin[1] + ray.z * ray.begin[2]);
	double c = pow(ray.begin[0], 2) + pow(ray.begin[1], 2) + pow(ray.begin[2], 2) + pow(centre_curve, 2) - pow(r, 2);

	double discriminant = pow(b, 2) - 4 * a * c;

	if (discriminant > 0)
	{
		if (r > 0) // если нужная нам часть вылуклая вправо
		{
			t = (-b + sqrt(discriminant)) / (2 * a);
			for (int i = 0; i < 3; ++i)
			{
				result[i] = ray.begin[i] + t * ray.cos[i];
			}
		}
		else //нужна часть сферы выпуклая влево
		{
			t = (-b - sqrt(discriminant)) / (2 * a);
			for (int i = 0; i < 3; ++i)
			{
				result[i] = ray.begin[i] + t * ray.cos[i];
			}
		}

	}
	else // если луч не соприкасается со сферой или идет по касательной - он не меняется
	{
		return ray
	}
	return result;
}

std::array<double>[3] Lense::refraction(Ray& ray, double r)
{
	result.begin = intersection(ray, r);
	std::array<double>[3] normal; //наверное потом унесу все в  функции операций для векторов
	normal[0] = centre[0] + boost::math::sign(r) * sqrt(pow(r, 2) + pow(length / 2, 2)) - result.begin[0]; 
	normal[1] = -result.begin[1];
	normal[2] = -result.begin[2];

	double sin_alpha = sqrt(1 - pow( (normal[0] * ray.cos[0] + normal[1] * ray.cos[1] + normal[2] * ray.cos[2]) / 
	( ( pow(normal[0],2) + pow(normal[1], 2) + pow(normal[2], 2) )*(pow(ray.cos[0], 2) + pow(ray.cos[1], 2) pow(ray.cos[2], 2), 2) ));

	double alpha = std::asin(sin_alpha);

	double sin_beta = sin_alpha / n; //закон преломления еее

	double beta = std::asin(sin_beta);


	//в итоге чтобы определить координаты преломившегося луча надо решить слу учитывающую компланарность падающего, 
	//преломленного лучей и нормали, угол паддающего с нормалью, угол преломленного с падающим
	

}

std::array<double>[3] slu(std::array<double>[3] & normal, std::array<double>[3] & fall, double alpha, double beta)
{

}



Ray Lense::refraction_in(Ray& ray)
{
	Ray result;

	result.begin = intersection(ray, r_left);
	
}