#pragma once
#include"sphere.h"
#include"material.h"
#include"bvh_node.h"
#include"texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include"box.h"
#include"constant_medium.h"

hittable* two_spheres()
{
	int nx, ny, nn;
	unsigned char* img = stbi_load("timg.jpg", &nx, &ny, &nn, 0);
	texture* text = new image_texture(img, nx, ny);
	hittable** list = new hittable * [2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new metal(vec3(0.65, 0.65, 0.65), 0.075));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(text));
	return new hittable_list(list, 2);
}

hittable* cornell_box()
{
	hittable** list = new hittable * [8];
	int i = 0;

	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));

	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

	hittable* b1 = new box(vec3(130, 0, 65), vec3(295, 165, 230), white);
	hittable* b2 = new box(vec3(265, 0, 295), vec3(430, 330, 460), white);
	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1, 1, 1)));
	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0, 0, 0)));

	return new hittable_list(list, i);
}



hittable* random_scene()
{
	int n = 500;
	hittable** list = new hittable * [n + 1];
	texture* checker = new checker_texture(
		new constant_texture(vec3(0.2, 0.3, 0.1)),
		new constant_texture(vec3(0.9, 0.9, 0.9))
	);
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -11; a < 11; a++)
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					list[i++] = new moving_sphere(
						center,
						center + vec3(0, 0.5 * random_double(), 0),
						0, 1, 0.2,
						new lambertian(
							new constant_texture(
								vec3(random_double() * random_double(),
									random_double() * random_double(),
									random_double() * random_double())
							))
					);
				}
				else if (choose_mat < 0.95)
				{
					list[i++] = new sphere(center, 0.2, new metal(
						vec3(0.5 * (1 + random_double()),
							0.5 * (1 + random_double()),
							0.5 * (1 + random_double())), 0.5 * random_double()
					));
				}
				else
				{
					list[i++] = new  sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}

	list[i++] = new sphere(vec3(0, 1, 0), 1, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1,
		new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1, new metal(vec3(0.7, 0.6, 0.5), 0));

	return new bvh_node(list, i, 0, 0);
}

