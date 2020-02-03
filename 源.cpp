#include<fstream>
#include<iostream>
#include"random.h"
#include"camera.h"
#include"arts.h"

inline void out_color(std::fstream& fs, int r, int g, int b)
{
	fs << r << ' ' << g << ' ' << b << '\n';
}

vec3 color(const ray& r, hittable* world,int depth)
{
	
	hit_record rec;
	if (world->hit(r, 0.01, 50000.0, rec))
	{
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return emitted + attenuation * color(scattered, world, depth + 1);
		else return emitted;
	}
	else
		return vec3(0,0,0);
}

int main()
{
	int nx = 600;
	int ny = 600;
	int ns = 2000;

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dis_to_focus = 10.0;
	camera cam(lookfrom,lookat,vec3(0,1,0),40,float(nx)/float(ny),0,dis_to_focus,0.0,1.0);

	hittable* world = cornell_box();

	std::fstream fs("img.ppm");
	fs << "P3\n" << nx << ' ' << ny << "\n255\n";
	for(int y=ny-1;y>=0;y--)
		for (int x = 0; x < nx; x++)
		{
			vec3 col(0, 0, 0);
			for (int i = 0; i < ns; i++)
			{
				float u = float(x + random_double()) / float(nx);
				float v = float(y + random_double()) / float(ny);
				col += color(cam.get_ray(u, v), world, 0);
			}
			col /= float(ns);
			out_color(fs, int(255.99 * sqrt(col.r())), 
						  int(255.99 * sqrt(col.g())), 
						  int(255.99 * sqrt(col.b())));
		}
	fs.close();
	return 0;
}