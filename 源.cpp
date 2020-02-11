#include<fstream>
#include<iostream>
#include"random.h"
#include"camera.h"
#include"arts.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"

const int nx = 600;
const int ny = 600;
char pic[ny * nx * 3];

inline void out_color(int x, int y, int r, int g, int b)
{
	pic[y * ny*3 + x * 3] = r;
	pic[y * ny *3+ x * 3 + 1] = g;
	pic[y * ny *3+ x * 3 + 2] = b;
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
	int ns = 50;

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	float dis_to_focus = 10.0;
	camera cam(lookfrom,lookat,vec3(0,1,0),40,float(nx)/float(ny),0,dis_to_focus,0.0,1.0);

	hittable* world = cornell_box();

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
			out_color(x,y,int(255.99 * sqrt(col.r())), 
						  int(255.99 * sqrt(col.g())), 
						  int(255.99 * sqrt(col.b())));
		}
	stbi_flip_vertically_on_write(1);
	stbi_write_jpg((const char*)"img.jpg", nx, ny, 3, (const void*)pic, 0);
	return 0;
}