
#include "OpticsClasses.hpp"
//
//
namespace Opticsproject
{
    Source::Source(json& j)
    {
        centre = dot(dim);
        auto centre_v = j["centre"].get<std::vector<double>>(); 
        for (size_t i = 0; i < centre_v.size(); ++i)
        {
            centre[i] = centre_v[i];
        }
        
        rgb = j["rgb"].get<std::vector<int>>(); 
    }

    std::vector<Ray> Source::produce_ray(double ap, size_t n) //ap - tg alpha - максимального угла под которым может вылететь
    {
        std::random_device seed;
        std::default_random_engine dre(seed());
        std::uniform_real_distribution<double> yzdis(-ap, ap);//TODO - несимметричный случац

        std::vector<Ray> result;

        Ray ray;
        ray.begin = centre;
        ray.n = 1;
        ray.rgb = rgb;

        for (int i = 0; i < n; ++i)
        {
            ray.cos[0] = 1.0;
            ray.cos[1] = yzdis(dre) * ray.cos[0];
            ray.cos[2] = yzdis(dre) * ray.cos[0];

            ray.cos /= sqrt(boost::numeric::ublas::inner_prod(ray.cos, ray.cos));

            result.push_back(ray);
        }

        return result;
    }
}