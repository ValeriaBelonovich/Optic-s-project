
#include "OpticsClasses.hpp"


namespace Opticsproject
{
	Lense::Lense(json& j)
	{
		r_left = j["R1"];
		r_right = j["R2"];
		length = j["l"];
		n = j["n"];
		centre = dot(dim);
		auto centre_v = j["centre"].get<std::vector<double>>();//подумать как доставать из джейсона вектор
		for (size_t i=0; i < centre_v.size(); ++i)
		{
			centre[i] = centre_v[i];
		}

	}
//
//	double Lense::aperture(dot s)//исключение &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//	{
//		return 1;
//	}
//
//
dot Lense::intersection(Ray& ray, double r)
{
		dot result = ray.begin;

		dot centre_curve = centre;
		centre_curve[0] += boost::math::sign(r) * sqrt(pow(r, 2) - pow(length / 2, 2));

		double a = boost::numeric::ublas::inner_prod(ray.cos, ray.cos);
		double b = 2 * boost::numeric::ublas::inner_prod(ray.begin - centre_curve,ray.cos);
		double c = boost::numeric::ublas::inner_prod(ray.begin - centre_curve, ray.begin - centre_curve) - pow(r, 2);

		double discriminant = b * b - 4 * a * c;

		//std::cout << discriminant << std::endl;


		if (discriminant >= eps)
		{
			double t;
			if (r > 0) //если линзы выгнута влево
			{
				t = (-b - sqrt(discriminant)) / (2 * a);
				if (t > eps)
				{
					if ((result[0] + ray.cos[0] * t) < result[0] + length/2)
					{
						result += ray.cos * t;
						return result;
					}
					else
					{
						return result;
					}
				}
			}
			else
			{
				t = (-b + sqrt(discriminant)) / (2 * a);
				if ((result[0] + ray.cos[0] * t) < result[0] + length / 2)
				{
					result += ray.cos * t;
					return result;
				}
				else
				{
					return result;
				}
			}
		}
		else return result;//выбросить исключение???	
}

dot Lense::intersection(Ray& ray)
{
		Ray result;
		auto i_left = intersection(ray, r_left);
		auto i_right = intersection(ray, r_right);
		double dist_left = boost::numeric::ublas::inner_prod(ray.begin - i_left, ray.begin - i_left);
		double dist_right = boost::numeric::ublas::inner_prod(ray.begin - i_right, ray.begin - i_right);

		if (dist_right <= dist_left&& dist_right >eps) return i_right;
		if (dist_left > eps) return i_left;
		return i_right;
}

Ray Lense::refraction(Ray& ray,double r) 
{
		Ray result = ray;
		dot centre_curve = centre;// радиус кривизны
		auto i = intersection(ray,r);
		centre_curve[0] += boost::math::sign(r) * sqrt(pow(r, 2) - pow(length / 2, 2));
		vector normal = (centre_curve - i)/ 
			std::sqrt(boost::numeric::ublas::inner_prod(centre_curve - i, centre_curve - i)); 

		double alpha = std::acos(std::abs(boost::numeric::ublas::inner_prod(ray.cos, normal)));
		double beta;

		if (ray.n == n) // то луч движется в линзе и должен выйти ??? можно ли равно в дабле юзать или лучше через разность и эписилон???
		{
			try
			{

				beta = std::asin(n * sin(alpha));// TODO выбравсываем исключение при ПВО
				ray.n = 1;
			}
			catch (...)
			{
				throw;
			}
		}
		else
		{
			beta = std::asin(sin(alpha) / n);
			ray.n = n;
		}

		result.begin = intersection(ray, r);
		result.cos = slu(alpha, beta, normal, ray.cos);
		result.cos /= std::sqrt(boost::numeric::ublas::inner_prod(result.cos, result.cos));
		ray.end = result.begin;
		//ray.send(); //TODO это оставить, просто процедуру реализовать
		return result;
	
}

Ray Lense::refraction(Ray& ray)
{
		if (boost::numeric::ublas::norm_1(intersection(ray) - intersection(ray, r_left)) < eps)
		{
			return refraction(ray, r_left);
		}
		else
		{
			return refraction(ray, r_right);
		}
	
}

	vector Lense::slu(double alpha, double beta, vector& normal, vector& ray)
	{
		//приводим к треугольному виду матрицу
		//      rayx          rayy          rayz      | cos(alpha-beta)
		//      nx          ny          nz            | cos(beta)
		// ry*nz-rz*ny rz*nx-rx*nz rx*ny-ry*nx        | 0

		boost::numeric::ublas::matrix<double> m(3, 3);
		
		for (int i = 0; i < 3; ++i)
		{
			m(0, i) = ray[i];
			m(1, i) = normal[i];
			m(2, i) = ray[(i + 1) % dim] * normal[(i + 2) % dim] - ray[(i + 2) % dim] * normal[(i + 1) % dim];
		}

		auto tm = boost::numeric::ublas::triangular_adaptor<boost::numeric::ublas::matrix<double>, boost::numeric::ublas::lower>(m);

		vector angles(3);

		angles[0] = cos(alpha - beta);
		angles[1] = cos(beta);
		angles[2] = 0.0;

		auto s = boost::numeric::ublas::solve(tm, angles, boost::numeric::ublas::lower_tag());

		return s;

		//решаем через boost 

		//profit


	}
}
