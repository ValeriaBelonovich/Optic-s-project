
#include "OpticsClasses.hpp"


namespace Opticsproject
{
	System::System(json& j)
	{
		std::cout << "System init started"<<std::endl;
		for (auto i : j["lens"])
		{
			system.emplace_back(std::make_unique<Lense>(i));
		}

		for (auto i : j["sour"])
		{
			sources.push_back(Source(i));
		}

		dx = j["boarders"]["dx"];
		dy = j["boarders"]["dy"];
		dz = j["boarders"]["dz"];

		std::cout << "System init ended" << std::endl;
	};

	bool System::boarders(Ray& ray)
	{
		//Ray result = ray;
		double t_1 = (dx - ray.begin[0]) / ray.cos[0];
		if (boost::math::sign(t_1) < 0)
		{
			t_1 = (-dx - ray.begin[0]) / ray.cos[0];
		}
		auto t_2 = (dy - ray.begin[1]) / ray.cos[1];
		if (boost::math::sign(t_2) < 0)
		{
			t_2 = (-dy - ray.begin[1]) / ray.cos[1];
		}

		t_1 = (t_1<t_2) ? t_1 : t_2;

		ray.end = ray.begin + t_1 * ray.cos;
		{
			std::scoped_lock<std::mutex> rays_lock(m_send);
			ray.send(r);
		}

		return true;
	}

	void System::run()
	{
		std::cout << "System run started" << std::endl;
		size_t discr = 10;//задается
		double aperture_temp = 0.5;  //TODO сделать функцию апертуры
		
		for (size_t i = 0; i < sources.size(); ++i)
		{
			auto s = sources[i].produce_ray(aperture_temp, discr);
			rays.insert(rays.begin(), s.begin(), s.end());
		}

		//trace();
		std::cout << "calculation started" << std::endl;
		boost::asio::thread_pool pool(std::thread::hardware_concurrency() - 1);
		while (!rays.empty())
		{
			boost::asio::post(pool,boost::bind(&System::trace,this));
		}
		pool.join();
		std::cout << "calculation ended, run ended" << std::endl;

		return;
	}

	//double System::saperture()
	//{
	//	//for(auto i : system) aperture();
	//	return 0.5;
	//}

	void System::trace()
	{
		try {
			Ray ray;
			{
				std::scoped_lock<std::mutex> rays_lock(m_rays);
				if (!rays.empty())
				{
					ray = rays.back();
					rays.pop_back();
				}
				else
				{
					return;
				}
			}

			{
				std::shared_lock<std::shared_mutex> rays_lock(m_system);
				//Lense lense;
				double n = 4 * (pow(dx, 2) + pow(dy, 2)+ pow(dz, 2));
				double min_dist = n;//задается границами n
				bool q = false;
				while (!q) 
				{
					Ray result;
					int current = -1;
					for (size_t i = 0; i < system.size(); ++i) //проходимся по системе (посмотреть алгоритм стл)
					{
						double dist = boost::numeric::ublas::inner_prod
						(system[i]->intersection(ray) - ray.begin, system[i]->intersection(ray) - ray.begin);

						if (min_dist > dist && dist > eps)
						{
							min_dist = dist;
							current = i;
						}
					}
					if (current > -1)
					{

						bool b;
						{
							std::scoped_lock<std::mutex> rays_lock(m_send);
							b = system[current]->end(ray);
						}
						if (b)
						{
							ray.send(r);
							return;
						}
						try {
							std::scoped_lock<std::mutex> rays_lock(m_send);

							{
								result = system[current]->refraction(ray);
								ray.end = result.begin;
								ray.send(r);
							}
						}
						catch (...)
						{
							return;
						}
						

						ray = result;
					}
					else
					{
						q = boarders(ray); //по хорошему запихать границы тоже в объекты
					}

				}
			}

			return;
		}
		catch (...)
		{
			return;
		}

	}
	
}