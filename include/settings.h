#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <string>

namespace GOFL
{
	struct TransparentHash {
		using is_transparent = std::true_type;

		template <typename T>
		_NODISCARD size_t operator()(const T& key) const {
			return std::hash<T>{}(key);
		}
	};

	struct TransparentEqual {
		using is_transparent = std::true_type;

		template <typename T1, typename T2>
		_NODISCARD bool operator()(const T1& lhs, const T2& rhs) const {
			return lhs == rhs;
		}
	};

	class Settings 
	{
		struct ArgName {
			static constexpr std::string_view window_width  = "-windowwidth";
			static constexpr std::string_view window_height = "-windowheight";
			static constexpr std::string_view field_width   = "-fieldwidth";
			static constexpr std::string_view field_height  = "-fieldheight";
			static constexpr std::string_view tickrate      = "-tickrate";

			static inline const std::unordered_set <std::string_view, TransparentHash, TransparentEqual> all = {
				window_width,
				window_height,
				field_width,
				field_height,
				tickrate
			};
		};

		static void x_invalid_param_value(std::string_view name, std::string const& value) {
			throw std::invalid_argument("Invalid param value: '" + std::string{ name.substr(1) } + "' is " + value);
		}

		_NODISCARD static auto transform_args(int argc, char* argv[])
		{
			std::unordered_map <std::string_view, std::string, TransparentHash, TransparentEqual> result;
			result.reserve (std::min<size_t>(argc, ArgName::all.size()));

			for (int i = 1; i < argc; ++i)
			{
				std::string_view arg   = argv[i];
				auto			 eqOff = arg.find('=');

				if (eqOff != std::string_view::npos 
					&& eqOff < arg.size() - 1
				) {
					std::string_view pfx  = arg.substr(0, eqOff);				
					auto			 name = ArgName::all.find(pfx);

					if (name != ArgName::all.end()) {
						result.emplace(*name, arg.substr(eqOff + 1));
						continue;
					}
				}
			
				throw std::invalid_argument(std::string{ "Invalid launch param: " } + argv[i]);
			}

			return result;
		}

		_NODISCARD static std::pair <size_t, size_t> get_screen_resolution()
		{
			SDL_Init(SDL_INIT_VIDEO);
			SDL_DisplayMode dm;
			SDL_GetDesktopDisplayMode(0, &dm);
			SDL_Quit();

			return { dm.w, dm.h };
		}

	public:
		constexpr Settings() noexcept = default;

		Settings (int argc, char* argv[]) :
			Settings ()
		{
			auto args  = transform_args(argc, argv);
			auto apply = [&] (std::string_view name, size_t& var, size_t min = 1, size_t max = 128) 
			{
				auto param = args.find(name);

				if (param != args.end())
				{
					auto const& str = param->second;
					size_t val;

					try {
						val = std::stoull(str);
					} 
					catch (std::invalid_argument const&) {
						x_invalid_param_value(name, str);
					}
					catch (std::out_of_range const&) {
						x_invalid_param_value(name, str);
					}

					if (val >= min && val <= max)
						var = val;
					else
						x_invalid_param_value(name, str);
				}
			};

			auto const [screenWidth, screenHeight] = get_screen_resolution();

			apply (ArgName::field_width,  fieldWidth,  1, screenWidth);
			apply (ArgName::field_height, fieldHeight, 1, screenHeight);

			apply (ArgName::window_width,  windowWidth,  fieldWidth,  screenWidth);
			apply (ArgName::window_height, windowHeight, fieldHeight, screenHeight);

			apply (ArgName::tickrate, tickrate);
		}

		size_t windowWidth  = 800;
		size_t windowHeight = 800;
		size_t fieldWidth   = 25;
		size_t fieldHeight  = 25;
		size_t tickrate		= 3;
	};
}
#endif