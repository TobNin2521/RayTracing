#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "../Camera.h"
#include "../HitableList.h"
#include "../Material.h"
#include "../Random.h"
#include "../Sphere.h"
#include <float.h>
#include <fstream>
#include <stdlib.h>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:	
	virtual void OnUIRender() override
	{		
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) 
		{
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeigth = ImGui::GetContentRegionAvail().y;

		if(m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth() , (float)m_Image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();
		//Render();
	}

	Vec3 color(const Ray& r, Hitable* world, int depth) {
		HitRecord rec;
		if (world->Hit(r, 0.001, FLT_MAX, rec)) {
			Ray scattered;
			Vec3 attenuation;
			if (depth < 8 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered)) {
				return attenuation * color(scattered, world, depth + 1);
			}
			else {
				return Vec3(0, 0, 0);
			}
		}
		else {
			Vec3 unit_direction = unit_vector(r.direction());
			float t = 0.5 * (unit_direction.y() + 1.0);
			return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
		}
	}
	uint32_t createRGBA(int r, int g, int b, int a)
	{
		return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
			+ (a & 0xff);
	}

	Hitable* random_scene() {
		int n = 500;
		Hitable** list = new Hitable*[n + 1];
		list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
		int i = 1;
		for (int a = -8; a < 8; a++) {
			for (int b = -8; b < 8; b++) {
				float choose_mat = random_double();
				Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
				if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
					if (choose_mat < 0.8) {
						list[i++] = new Sphere(center, 0.2, new Lambertian(Vec3(random_double() * random_double(), 
							random_double() * random_double(), random_double() * random_double())));
					}
					else if (choose_mat < 0.95) {
						list[i++] = new Sphere(center, 0.2, new Metal(Vec3(0.5 * (1 + random_double()), 
							0.5 * (1 + random_double()), 
							0.5 * (1 + random_double())), 
							0.5 * random_double()));
					}
					else {
						list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
					}
				}
			}
		}

		list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
		list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
		list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

		return new HitableList(list, i);
	}

	void Render()
	{
		Timer timer;
		m_ViewportWidth = 800;
		m_ViewportHeigth = 480;
		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeigth != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeigth, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeigth];
		}

		int nx = m_ViewportWidth, ny = m_ViewportHeigth, index = 0, ns = 20;
				
		Hitable* world = random_scene();

		Vec3 lookfrom(13, 2, 3);
		Vec3 lookat(0, 0, 0);
		float dist_to_focus = 10;
		float aperture = 0.1;

		Camera cam(lookfrom, lookat, Vec3(0, 1, 0), 20, float(nx)/float(ny), aperture, dist_to_focus);
		
		std::string content;

		content = "P3\n" + std::to_string(nx) + " " + std::to_string(ny) + "\n255\n";

		for (int j = ny - 1; j >= 0; j--)
		{
			std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
			for (int i = 0; i < nx; i++)
			{
				Vec3 col(0, 0, 0);
				for (int s = 0; s < ns; s++) {
					float u = float(i + random_double()) / float(nx);
					float v = float(j + random_double()) / float(ny);
					Ray r = cam.get_ray(u, v);
					col += color(r, world, 0);
				}
				col /= float(ns);
				col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
				int ir = int(255.99f * col[0]);
				int ig = int(255.99f * col[1]);
				int ib = int(255.99f * col[2]);


				uint32_t c = createRGBA(ir, ig, ib, 0xff);
				content += std::to_string(ir) + " " + std::to_string(ig) + " " + std::to_string(ib) + "\n";

				m_ImageData[index] = c;
				index++;
			}
		}

		std::fstream file("image.ppm", std::ios_base::out);
		file << content;
		file.close();

		m_Image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	std::shared_ptr<Image> m_Image;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeigth = 0;
	uint32_t* m_ImageData = nullptr;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";
	//spec.ChildBg = { 44, 43, 56, 255 };
	//spec.ParentBg = { 44, 43, 65, 255 };
	//spec.Icon = "C:\\Users\\i0006683\\Documents\\Projects\\RayTracing\\bin\\Debug-windows-x86_64\\RayTracing\\icon.png";
	srand(time(NULL));

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	/*app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});*/
	return app;
}