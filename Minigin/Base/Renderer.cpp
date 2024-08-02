#include <stdexcept>
#include "Renderer.h"
#include "scene/SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>


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



	// Innitializing the ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);


	// 1) Make an empty buffer
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();


	SceneManager::GetInstance().Render();




	//3) Swap the buffer so it is shown
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

void dae::Renderer::RenderRect(float x, float y, float width, float height, SDL_Color color) const
{
	// Create SDL_Rect object for destination
	SDL_Rect dstRect{};
	dstRect.x = static_cast<int>(x);
	dstRect.y = static_cast<int>(y);
	dstRect.w = static_cast<int>(width);
	dstRect.h = static_cast<int>(height);


	SDL_SetRenderDrawColor(GetSDLRenderer(), color.r, color.g, color.b, color.a);

	// Render filled rectangle
	SDL_RenderFillRect(GetSDLRenderer(), &dstRect);
}

void dae::Renderer::RenderRectEdges(float x, float y, float width, float height, SDL_Color color) const
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), color.r, color.g, color.b, color.a);

	// Calculate corner positions
	glm::vec2 topLeft{ x, y };
	glm::vec2 topRight{ x + width, y };
	glm::vec2 bottomLeft{ x, y + height };
	glm::vec2 bottomRight{ x + width, y + height };


	// Render top edge
	SDL_RenderDrawLine(GetSDLRenderer(), static_cast<int>(topLeft.x), static_cast<int>(topLeft.y), static_cast<int>(topRight.x), static_cast<int>(topRight.y));

	// Render bottom edge
	SDL_RenderDrawLine(GetSDLRenderer(), static_cast<int>(bottomLeft.x), static_cast<int>(bottomLeft.y), static_cast<int>(bottomRight.x), static_cast<int>(bottomRight.y));

	// Render left edge
	SDL_RenderDrawLine(GetSDLRenderer(), static_cast<int>(topLeft.x), static_cast<int>(topLeft.y), static_cast<int>(bottomLeft.x), static_cast<int>(bottomLeft.y));

	// Render right edge
	SDL_RenderDrawLine(GetSDLRenderer(), static_cast<int>(topRight.x), static_cast<int>(topRight.y), static_cast<int>(bottomRight.x), static_cast<int>(bottomRight.y));
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

SDL_Window* dae::Renderer::GetWindow() const
{
	return m_window;
}
