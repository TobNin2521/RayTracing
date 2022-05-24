#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		if (ImGui::Button("Render")) 
		{
			Render();
		}
		ImGui::End();

		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeigth = ImGui::GetContentRegionAvail().y;

		if(m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth() , (float)m_Image->GetHeight() });

		ImGui::ShowDemoWindow();
	}

	void Render()
	{
		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeigth != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeigth, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeigth];
		}

		for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeigth; i++) 
		{
			m_ImageData[i] = 0xffff00ff;
		}

		m_Image->SetData(m_ImageData);
	}
private:
	std::shared_ptr<Image> m_Image;
	uint32_t m_ViewportWidth = 0;
	uint32_t m_ViewportHeigth = 0;
	uint32_t* m_ImageData = nullptr;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}