
#include "OpticsClasses.hpp"


namespace Opticsproject
{
	void Ray::send(json& r)
	{
		json temp;
		for (int i = 0; i < dim; ++i)
		{
			temp["begin"].push_back(begin[i]);
			temp["end"].push_back(end[i]);
		}

		temp["rgb"] = rgb;

		r.push_back(temp);
		return; 
	}
}