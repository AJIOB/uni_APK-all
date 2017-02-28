#include "StreamInput.h"

char Stream::GetOnlyYN(const std::string& infoText)
{
	char result;

	while (true)
	{
		OutputConsole(infoText +  " (Y - да, N - нет)");
		result = Stream::Get();

		if (result == 'y' || (result == 'n'))
		{
			result += 'A' - 'a';
			break;
		}

		if (result == 'Y' || (result == 'N'))
		{
			break;
		}

		OutputConsole("Извините, такой вариант не поддерживается. Пожалуйста, повторите ввод");
	}

	return result;
}

int Stream::InputEnum(const std::vector<std::string>& texts, int numOfTheFirstElementToShow)
{
	int result = 0;

	while (true)
	{
		//демонстрация вариантов для выбора
		std::cout << "Пожалуйста, введите цифру, соответсувующую вашему выбору:" << std::endl;
		for (long int i = numOfTheFirstElementToShow; i < static_cast<long int>(texts.size()); ++i)
		{
			std::cout << i << ": " << texts[i] << std::endl;
		}

		OutputConsole("Пожалуйста, сделайте свой выбор:");

		Input(result);

		if ((result >= numOfTheFirstElementToShow) && (result < static_cast<long int>(texts.size())))
		{
			Clear();
			break;
		}		

		Clear();
		ClearConsole();

		OutputWarning("Извините, такой вариант нельзя ввести. Пожалуйста, введите что-либо из нижепредложенных вариантов");
	}

	return result;
}

std::string Stream::Getpass(const char* prompt, bool show_asterisk)
{
	const char BACKSPACE=8;
	const char RETURN=13;

	std::string password;
	unsigned char ch=0;

	std::cout <<prompt<< std::endl;

	do
	{
		ch = _getch();
		if (ch == RETURN)
		{
			break;
		}

		if (ch == BACKSPACE)
		{
			if ((password.length() == 0))
			{
				continue;
			}

			password.pop_back();

			//переввод новых *
			if (show_asterisk)
			{
				std::cout << "\r";
				//затирание старого пробела
				for (auto i = 0; i <= password.size(); i++)
				{
					std::cout << " ";
				}

				std::cout << "\r";
				for (auto i = 0; i < password.size(); i++)
				{
					std::cout << "*";
				}
			}

			continue;
		}

		password.push_back(ch);
		if (show_asterisk)
		{
			std::cout << "*";
		}

	} while (true);

	std::cout << std::endl;
	return password;
}
