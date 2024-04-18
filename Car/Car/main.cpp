#include<iostream>
using namespace std;

#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 120

class Tank
{
	double fuel_level;
	const int VOLUME;
public:
	const int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double amount)
	{
		if (amount < 0)return;
		if (fuel_level + amount < VOLUME)fuel_level += amount;
		else fuel_level = VOLUME;
	}
	double give_fuel(double amount)
	{
		if (amount < 0)return fuel_level;
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	Tank(int volume):VOLUME
		(
			volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
			volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
			volume
		),
		fuel_level(0)
	{
		/*if (volume < MIN_TANK_VOLUME)volume = MIN_TANK_VOLUME;
		if (volume > MAX_TANK_VOLUME)volume = MAX_TANK_VOLUME;
		this->VOLUME = volume;*/
		//this->fuel_level = 0;
		cout << "Tank is ready " << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over " << this << endl;
	}
	void info()const
	{
		cout << "Tank volume: " << VOLUME << " liters.\n";
		cout << "Fuel level:  " << fuel_level << " liters.\n";
	}
};

#define MIN_ENGINE_CONSUMPTION	3
#define MAX_ENGINE_CONSUMPTION	30
class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;
public:
	double get_consumption()const
	{
		return CONSUMPTION;
	}
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	Engine(double consumption)
		:CONSUMPTION
		(
			consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
			consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
			consumption
		),
		consumption_per_second(CONSUMPTION * 3e-5),
		is_started(false)
	{
		//consumption_per_second = CONSUMPTION * 3e-5;
		cout << "Engine is ready: " << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over: " << this << endl;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption: " << CONSUMPTION << " liters/100km" << endl;
		cout << "Consumption: " << consumption_per_second << " liters/100km" << endl;
		cout << "Engine is " << (is_started ? "started" : "stopped") << endl;
	}
};

//#define TANK_CHECK
//#define (Определить)
//директива #define создает макроопределение (макрос)
#define ENGINE

void main()
{
	setlocale(LC_ALL, "");

#if defined TANK_CHECK
	//если определено TANK_CHECK, то нижеследующий код до диркутивы #endif будет виден компилятору.
	Tank tank(85);
	int fuel;
	do
	{
		cout << "Введите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);
#endif // TANK_CHECK

	Engine engine(10);
	engine.info();
}

/*
------------------------

Pointer
& - Address-of operator (возвращает адрес своего операнда)
* - Dereference operator (возвращает значение по адресу)

Reference

------------------------
*/