#include <iostream>

#ifdef _WIN32
#include <Windows.h>
HANDLE hConsole;
#endif

#include "Engine/Core.h"

namespace Console 
{
#ifdef _WIN32
	enum class ConsoleColour
	{
		ForegroundBlack		 = 0, 
		ForegroundDarkBlue	 =					   									      FOREGROUND_BLUE,
		ForegroundDarkGreen	 =					   				       FOREGROUND_GREEN,
		ForegroundDarkCyan	 =					   				       FOREGROUND_GREEN | FOREGROUND_BLUE,
		ForegroundDarkRed	 =					      FOREGROUND_RED,
		ForegroundDarkPurple =						  FOREGROUND_RED |					  FOREGROUND_BLUE,
		ForegroundDarkYellow =						  FOREGROUND_RED | FOREGROUND_GREEN,
		ForegroundGrey		 =						  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		ForegroundBlue	     = FOREGROUND_INTENSITY |									  FOREGROUND_BLUE,
		ForegroundGreen	     = FOREGROUND_INTENSITY |				   FOREGROUND_GREEN,
		ForegroundCyan	     = FOREGROUND_INTENSITY |				   FOREGROUND_GREEN | FOREGROUND_BLUE,
		ForegroundRed	     = FOREGROUND_INTENSITY | FOREGROUND_RED,
		ForegroundPurple	 = FOREGROUND_INTENSITY | FOREGROUND_RED |					  FOREGROUND_BLUE,
		ForegroundYellow	 = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		ForegroundWhite	     = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		BackgroundBlack		 = 0, 
		BackgroundDarkBlue	 =					   									      BACKGROUND_BLUE,
		BackgroundDarkGreen	 =					   				       BACKGROUND_GREEN,
		BackgroundDarkCyan	 =					   				       BACKGROUND_GREEN | BACKGROUND_BLUE,
		BackgroundDarkRed	 =					      BACKGROUND_RED,
		BackgroundDarkPurple =						  BACKGROUND_RED |					  BACKGROUND_BLUE,
		BackgroundDarkYellow =						  BACKGROUND_RED | BACKGROUND_GREEN,
		BackgroundGrey		 =						  BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		BackgroundBlue	     = BACKGROUND_INTENSITY |									  BACKGROUND_BLUE,
		BackgroundGreen	     = BACKGROUND_INTENSITY |				   BACKGROUND_GREEN,
		BackgroundCyan	     = BACKGROUND_INTENSITY |				   BACKGROUND_GREEN | BACKGROUND_BLUE,
		BackgroundRed	     = BACKGROUND_INTENSITY | BACKGROUND_RED,
		BackgroundPurple	 = BACKGROUND_INTENSITY | BACKGROUND_RED |					  BACKGROUND_BLUE,
		BackgroundYellow	 = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		BackgroundWhite	     = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	};
#else
	enum class ConsoleColour
	{
		ForegroundBlack, ForegroundDarkBlue, ForegroundDarkGreen, ForegroundDarkCyan, ForegroundDarkRed, ForegroundDarkPurple, ForegroundDarkYellow,
		ForegroundGrey, ForegroundBlue, ForegroundGreen, ForegroundCyan, ForegroundRed, ForegroundPurple, ForegroundYellow, ForegroundWhite,
		BackgroundBlack, BackgroundDarkBlue, BackgroundDarkGreen, BackgroundDarkCyan, BackgroundDarkRed, BackgroundDarkPurple, BackgroundDarkYellow,
		BackgroundGrey, BackgroundBlue, BackgroundGreen, BackgroundCyan, BackgroundRed, BackgroundPurple, BackgroundYellow, BackgroundWhite,
	};
#endif

	static void SetColour(int val)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, val);
#endif
	}

	void _OnStart()
	{
#ifdef _WIN32
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	void _OnExit()
	{
		SetColour((int)ConsoleColour::ForegroundWhite | (int)ConsoleColour::BackgroundBlack);
	}

	void Log(const std::string& message)
	{
		std::cout << message << std::endl;
	}

	void LogError(const std::string& message)
	{
		SetColour((int)ConsoleColour::ForegroundDarkRed);
		std::cout << message << std::endl;
		SetColour((int)ConsoleColour::ForegroundWhite);
	}

	void LogWarning(const std::string& message)
	{
		SetColour((int)ConsoleColour::ForegroundYellow);
		std::cout << message << std::endl;
		SetColour((int)ConsoleColour::ForegroundWhite);
	}
}