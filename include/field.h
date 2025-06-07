#ifndef FIELD_HEADER
#define FIELD_HEADER

#include "settings.h"

namespace GOFL
{
	class Cell
	{
	public:
		Cell () noexcept :
			isAlive     (false),
			willBeAlive (false),
			hasChanged  (true)
		{}

		void accept() noexcept {
			hasChanged = willBeAlive != isAlive;
			isAlive    = willBeAlive;
		}

		void make_alive() noexcept {
			willBeAlive = true;
		}

		void make_dead() noexcept {
			willBeAlive = false;
		}

		_NODISCARD bool is_alive() const noexcept {
			return isAlive;
		}

		_NODISCARD bool has_changed() const noexcept {
			return hasChanged;
		}

	private:
		bool isAlive	 : 1;
		bool willBeAlive : 1;
		bool hasChanged  : 1;
	};

	class Field
{
public:
	Field (std::shared_ptr <Settings const> settings) :
		myWidth  (settings->fieldWidth),
		myHeight (settings->fieldHeight)
	{
		myField.resize(myWidth);

		for (auto& col : myField)
			col.resize(myHeight);
	}

	void update()
	{
		for (size_t i = 0; i < myWidth;  ++i)
		for (size_t j = 0; j < myHeight; ++j)
		{
			int neighbors = 0;

			for (int di = -1; di <= 1; ++di)
			for (int dj = -1; dj <= 1; ++dj)
			{
				if (di == 0 && dj == 0) continue;

				if (myField [(i + di + myWidth) % myWidth] [(j + dj + myHeight) % myHeight].is_alive())
					if (++neighbors > 3) break;
			}

			auto& cell = myField[i][j];

			if (neighbors < 2 || neighbors > 3)
				cell.make_dead();

			else if (neighbors == 3)
				cell.make_alive();
		}

		for (size_t i = 0; i < myWidth;  ++i)
		for (size_t j = 0; j < myHeight; ++j)
			myField[i][j].accept();
	}

	void set_state(bool alive, size_t x, size_t y) noexcept
	{
		if (x < myWidth && y < myHeight)
		{
			auto& cell = myField[x][y];

			alive ? cell.make_alive() : cell.make_dead();
			cell.accept();
		}
	}

	_NODISCARD size_t get_width() const noexcept {
		return myWidth;
	}

	_NODISCARD size_t get_height() const noexcept {
		return myHeight;
	}

	_NODISCARD bool get_state(size_t x, size_t y) const 
	{
		if (x < myWidth && y < myHeight)
			return myField[x][y].is_alive();
		else
			throw std::invalid_argument("out of the field size");
	}

private:
	std::vector <std::vector <Cell>> myField;
	size_t myWidth;
	size_t myHeight;
};	
}
#endif