/*

*/

#include "MyLibrary.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	do
	{
		system("cls");

		defaultFunction();
		std::cout << "Если хотите повторить, введите '+'" << std::endl;
		fflush(stdin);
	} while (getch() == '+');

	programEnd();
	return 0;
}