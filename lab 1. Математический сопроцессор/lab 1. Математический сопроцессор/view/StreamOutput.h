#pragma once

#include <string>
#include <iostream>
#include <vector>

void OutputConsole(const std::string&);
void OutputWarning(const std::string&);/*
void OutputError(const std::string&);
void OutputLog(const std::string&, const bool isCloseFile = false);
*/
std::string OutputEnum(const std::vector<std::string>& texts, const int& enumToShow);

inline void ClearConsole()
{
	system("cls");
}

inline void PauseConsole()
{
	system("pause");
}