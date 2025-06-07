#ifndef GAME_HEADER
#define GAME_HEADER

#include <chrono>
#include <thread>

#include "settings.h"
#include "field.h"
#include "window.h"

namespace GOFL
{
	class Game
	{
	public:
		Game(std::shared_ptr <Settings> settings) :
			mySettings (settings),
			myField	   (settings),
			myWindow   (settings),
			myPrevUpdateTime (std::chrono::system_clock::now())
		{}

		void run()
		{
			while (!myWindow.is_closing())
			{
				auto sleepDur = std::chrono::duration<float>(1.f / mySettings->tickrate);
				auto currTime = std::chrono::system_clock::now();
				bool updated = false;

				if (!myWindow.is_paused() && currTime - myPrevUpdateTime > sleepDur)
				{
					myPrevUpdateTime = currTime;
					myField.update();
					updated = true;
				}

				myWindow.update(myField, updated);
			}
		}

	private:
		std::shared_ptr <Settings> mySettings;

		Field  myField;
		Window myWindow;

		std::chrono::system_clock::time_point myPrevUpdateTime;
	};
}

#endif