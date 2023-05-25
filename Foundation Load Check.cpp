#include <iostream>
#include <string>
#include <algorithm>
#include <Windows.h>
#include <limits>

//A class for the entire case, width and length of the mats, outrigger load in tonnes, outrigger load in kN, ground pressures

std::string to_lowercase(std::string& str)
{
	//std::string lower_case_str = str;
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}


class LoadCheck
{
public:
	LoadCheck() {};
	//LoadCheck(double padWidth, double padLength, double imposedLoad) {};
	void TonneToForce(double imposedTonne);
	void setPadSize();
	void setImposedLoad();
	void setImposedForce();
	void printGroundPressure();
	void resetVariables();
	double getIntFromUser();
private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	double m_imposedForce{}; //in kN
	double m_padWidth{};
	double m_padLength{};
	double m_groundPressure{};
	double m_imposedTonnes{};
	static constexpr double gravityAcc{ 9.81 };
};

double LoadCheck::getIntFromUser()
{
	double number{};
	while (true)
	{
		std::cin >> number;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(10000, '\n'); //numeric limits is not working
		}
		else
		{
			std::cin.ignore(10000, '\n');
			return number;
		}
	}
}

void LoadCheck::TonneToForce(double imposedTonne)
{
	this->m_imposedTonnes = imposedTonne;
	imposedTonne *= 1000; //tonnes to kg
	imposedTonne *= this->gravityAcc; //convert to N
	m_imposedForce = imposedTonne / 1000;
}

void LoadCheck::setPadSize()
{
	std::cout << "Enter the pad width in meters\n";
	this->m_padWidth = this->getIntFromUser();
	//std::cin >> this->m_padWidth;
	std::cout << "Enter the pad length in meters\n";
	this->m_padLength = this->getIntFromUser();
	//std::cin >> this->m_padLength;
}

void LoadCheck::setImposedLoad()
{
	std::string userChoice{};
	std::cout << "Is the imposed load currently in KiloNewtons[kN] or tonnes[t]?\n";
	do //validate input
	{
		std::getline(std::cin, userChoice);
		userChoice = to_lowercase(userChoice);
	} while (userChoice != "kilonewtons" && userChoice != "kn" && userChoice != "tonnes" && userChoice != "t");
	if (userChoice == "tonnes" || userChoice == "t") //convert from tonnes to kN
	{
		std::cout << "Enter the maximum provided outrigger load in tonnes\n";
		double tonnes{ this->getIntFromUser() };
		std::cout << "Converting tonnes to kN...\n";
		TonneToForce(tonnes); //converts provided tonnes into force
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout << this->m_imposedForce << "kN\n";
	}
	else(setImposedForce());
		
}

void LoadCheck::setImposedForce()
{
	std::cout << "Enter the maximum provided outrigger load in kN\n";
	this->m_imposedForce = this->getIntFromUser();
	//std::cin >> this->m_imposedForce;
	this->m_imposedTonnes = this->m_imposedForce / this->gravityAcc;
}

void LoadCheck::printGroundPressure() //divide imposed force by area of mats
{
	this->m_groundPressure = this->m_imposedForce / (this->m_padLength * this->m_padWidth);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::cout << "\nParameters:\nPad Width = " << this->m_padWidth << 'm' << "\nPad Length = " << this->m_padLength <<  'm' << "\nImposed Tonneage = " << this->m_imposedTonnes << 't' <<  "\nImposed Force = " << this->m_imposedForce << "kN" << " \n\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	std::cout << "Ground bearing pressure = " << this->m_groundPressure << "kN/m2" << " || " << this->m_imposedTonnes / (this->m_padLength * this->m_padWidth) << "t/m2\n\n";
}

void LoadCheck::resetVariables()
{
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	std::cout << "Resetting Parameters\n-------------------------------------------------------------------\n\n";
	this->m_groundPressure = 0;
	this->m_imposedForce = 0;
	this->m_imposedTonnes = 0;
	this->m_padLength = 0;
	this->m_padWidth = 0;
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


int main()
{
	LoadCheck check{};
	while (true)
	{
		check.setPadSize(); //get the proposed pad size
		check.setImposedLoad(); //get the load the pad will be taking
		check.printGroundPressure();
		check.resetVariables();

		Sleep(50);
	}
	
}


