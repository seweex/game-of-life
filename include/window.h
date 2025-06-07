#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "settings.h"

namespace GOFL
{
	class Window
	{
		static constexpr std::string_view title		   = "Game of Life";
		static constexpr std::string_view title_paused = "Game of Life (Paused)";

		static constexpr int acceleration_factor = 2;

		_NODISCARD static auto make_window (size_t width, size_t height) 
		{
			auto handle = SDL_CreateWindow(title_paused.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

			if (handle == nullptr)
				throw std::invalid_argument(std::string{ "Failed to create a window: " } + SDL_GetError());
			else
				return handle;
		}

		_NODISCARD static auto make_renderer(SDL_Window* wnd)
		{
			auto handle = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (handle == nullptr)
				throw std::invalid_argument(std::string{ "Failed to create a renderer: " } + SDL_GetError());
			else
				return handle;
		}

	public:
		Window (std::shared_ptr <Settings> settings) :
			mySettings (settings),
			myWidth    (settings->windowWidth),
			myHeight   (settings->windowHeight),
			myWindow   (make_window(myWidth, myHeight)),
			myRenderer (make_renderer(myWindow))
		{
			SDL_SetRenderDrawColor (myRenderer, 152, 255, 152, 255);
			SDL_RenderClear		   (myRenderer);
			SDL_RenderPresent	   (myRenderer);
		}

		~Window() {
			SDL_DestroyRenderer(myRenderer);
			SDL_DestroyWindow(myWindow);
			SDL_Quit();
		}

		void update (Field& field, bool doDraw)
		{ 
			auto const cellWidth  = myWidth  / field.get_width();
			auto const cellHeight = myHeight / field.get_height();

			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT) {
					isClosing = true;
					return;
				}

				else if (event.type == SDL_KEYDOWN)
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						isClosing = true;
						return;

					case SDLK_SPACE:
						isPaused = !isPaused;
						SDL_SetWindowTitle (myWindow, (isPaused ? title_paused : title).data());
						break;

					case SDLK_c:
						for (size_t i = 0; i < field.get_width();  ++i)
						for (size_t j = 0; j < field.get_height(); ++j)
							field.set_state(false, i, j);
					
						doDraw = true;
						break;

					case SDLK_q:
						mySettings->tickrate = std::max<size_t>(mySettings->tickrate / acceleration_factor, 1);
						break;

					case SDLK_e:
						mySettings->tickrate = std::min<size_t>(mySettings->tickrate * acceleration_factor, 1000);
						break;
					}
					
				else if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					int mouseX;
					int mouseY;

					SDL_GetMouseState(&mouseX, &mouseY);

					int gridX = mouseX / cellWidth;
					int gridY = mouseY / cellHeight;

					bool currState = field.get_state(gridX, gridY);
					field.set_state(!currState, gridX, gridY);

					doDraw = true;
				}
			}

			if (doDraw)
			{
				SDL_SetRenderDrawColor(myRenderer, 152, 255, 152, 255);
				SDL_RenderClear(myRenderer);

				SDL_SetRenderDrawColor(myRenderer, 227, 11, 92, 255);

				SDL_Rect cell;
				cell.w = cellWidth;
				cell.h = cellHeight;

				for (size_t i = 0; i < field.get_width();  ++i)
				for (size_t j = 0; j < field.get_height(); ++j)
					if (field.get_state(i, j))
					{
						cell.x = i * cellWidth;
						cell.y = j * cellHeight;

						SDL_RenderFillRect(myRenderer, &cell);
					}

				SDL_RenderPresent(myRenderer);
			}
		}

		_NODISCARD bool is_paused() const noexcept {
			return isPaused;
		}

		_NODISCARD bool is_closing() const noexcept {
			return isClosing;
		}

	private:
		std::shared_ptr <Settings> mySettings;

		size_t myWidth;
		size_t myHeight;

		SDL_Window*   myWindow;
		SDL_Renderer* myRenderer;

		bool isPaused  = true;
		bool isClosing = false;
	};
}
#endif