#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui_plot.h"
#include "TrashTheCache.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	SDL_Color color{ 62, 90, 111 };
	SetBackgroundColor(color);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();

}

void dae::Renderer::UpdateImGui(TrashTheCache* pExercise1, TrashTheCache* pExercise2)
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	bool windowActive = true;

	// // *** EXERCISE 1 WINDOW *****
	ImGui::SetNextWindowPos(ImVec2(30.f, 30.f));
	ImGui::SetNextWindowSize(ImVec2(300.f, 400.f));
	ImGui::Begin("Exercise1", &windowActive);

	if (m_Ex1ButtonPressed)
	{
		// Run it next frame
		m_ShowExercise1 = pExercise1->RunExercise(1);
	}

	// Specify how many samples we want
	//ImGui::InputInt("# samples", &pExercise1->GetSamples());
	// Slider instead of input so I can clamp the values between the desired ones
	ImGui::SliderInt("# samples", &pExercise1->GetSamples(), 10, 100); 
	if (ImGui::Button("Trash the cache"))
	{	
		// Show text and calculate the timings for Exercise1
		m_Ex1ButtonPressed = true;
		m_ShowExercise1 = false;
	}

	if (m_Ex1ButtonPressed)
	{
		ImGui::SameLine();   // Draw it next to the button
		ImGui::Text("Calculating ...");
	}

	if (m_ShowExercise1)
	{
		m_Ex1ButtonPressed = false;
		ImGui::PlotConfig confEx1;
		float* values{ pExercise1->GetTimingsValues() };
		confEx1.values.xs = m_XValues;
		confEx1.values.ys = values;
		confEx1.values.count = pExercise1->GetTotalTimings();
		confEx1.scale.min = 0;
		confEx1.scale.max = values[0];			// First value will be the biggest value
		confEx1.tooltip.show = true;
		confEx1.tooltip.format = "x=%.2f, y=%.2f";
		confEx1.grid_x.show = true;
		confEx1.grid_y.show = true;
		confEx1.frame_size = ImVec2(250, 200);
		confEx1.line_thickness = 2.f;

		ImGui::Plot("Exercise1", confEx1);
		
	}
	
	// *** EXERCISE 2 WINDOW *****	
	ImGui::SetNextWindowPos(ImVec2(350.f, 30.f));
	ImGui::SetNextWindowSize(ImVec2(400.f, 400.f));
	ImGui::Begin("Exercise2", &windowActive);

	if (m_Ex2ButtonPressed)
	{
		m_ShowExercise2 = pExercise2->RunExercise(2);
	}

	// Specify how many samples we want
	//ImGui::InputInt("# samples", &pExercise2->GetSamples());
	ImGui::SliderInt("# samples", &pExercise2->GetSamples(), 10, 50);
	if (ImGui::Button("Trash the cache with GameObjects3D"))
	{
		// Show text and calculate the timings for Exercise2
		m_ShowExercise2 = false;	
		m_Ex2ButtonPressed = true;
	}

	if (m_Ex2ButtonPressed)
	{
		ImGui::SameLine();   // Draw it next to the button
		ImGui::Text("Calculating ...");
	}

	if (m_ShowExercise2)
	{
		m_Ex2ButtonPressed = false;
		// Define the configuration options for the graph
		ImGui::PlotConfig confEx2;
		float* values{ pExercise2->GetTimingsValues() };
		confEx2.values.xs = m_XValues;
		confEx2.values.ys = values;
		confEx2.values.count = pExercise2->GetTotalTimings();
		confEx2.scale.min = 0;
		confEx2.scale.max = values[0];			// First value will be the biggest value
		confEx2.tooltip.show = true;
		confEx2.tooltip.format = "x=%.2f, y=%.2f";
		confEx2.grid_x.show = true;
		confEx2.grid_y.show = true;
		confEx2.frame_size = ImVec2(250, 200);
		confEx2.line_thickness = 2.f;

		ImGui::Plot("Exercise2", confEx2);

	}
	
	ImGui::End();		// Exercise2 End
	ImGui::End();		// Exercise1 End

}


void dae::Renderer::Render()
{
	
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer); 

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	SceneManager::GetInstance().Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
