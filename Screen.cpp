
//#include "OpticsClasses.hpp"
//
//
//namespace Opticsproject
//{
//	bool Screen::end(Ray& ray)
//	{
//
//		auto t = (centre[0] - ray.begin[0])/ray.cos[0]; //пересечение с плоскостью x=xэ
//		auto y = ray.begin[1] + t * ray.cos[1];
//		if ((y > centre[1] + length / 2) && (y < centre[1] - length / 2))
//		{
//			return false; 
//		}
//		auto z = ray.begin[2] + t * ray.cos[2];
//		if ((z > centre[2] + length / 2) && (z < centre[2] - length / 2))
//		{
//			return false; 
//		}
//
//		ray.end[0] = centre[0];
//		ray.end[1] = y;
//		ray.end[2] = z;
//
//
//		return true;
//	}
//
//}