#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

#include "StreamOutput.h"

namespace Stream
{
	void Clear();

	template<typename InputType>
	InputType Input();
	
	template<typename InputType>
	void Input(InputType& elem);

	char Get();
	char GetOnlyYN(const std::string& infoText);
	int InputEnum(const std::vector<std::string>& texts, int numOfTheFirstElementToShow = 1);

	std::string Getpass(const char *prompt, bool show_asterisk = true);
	/*
	template<typename InputType>
	InputType InputInRange(const std::string& shownText, const InputType& min, const InputType& max);*/
}



#define AJIOB_END_OF_LINE '\n'


//cpp part 1. inline

inline void Stream::Clear()
{
	std::cin.clear();

	std::streambuf* tmp = std::cin.rdbuf();

	while ( tmp->in_avail() != 0 )
	{
		//переход на следующий символ
		tmp->sbumpc();
    }
}

template<typename InputType>
InputType Stream::Input()
{
	InputType toReturn;

	while (true)
	{
		if (std::cin >> toReturn)
		{
			Clear();
			break;
		}
		OutputConsole("Ошибка ввода. Пожалуйста, повторите ввод");
		Clear();
	}
	return toReturn;
}

template<typename InputType>
void Stream::Input(InputType& elem)
{
	elem = Input<InputType>();
}

//for std::string
template<>
inline std::string Stream::Input()
{
	std::string buffer;

	while (true)
	{
		char tmp = std::cin.get();

		if (tmp == AJIOB_END_OF_LINE)
		{
			break;
		}

		buffer.push_back(tmp);
	}

	return buffer;
}
//end for std::string


inline char Stream::Get()
{
	return Input<char>();
}

#undef AJIOB_END_OF_LINE