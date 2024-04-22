#include<iostream>
#include<conio.h>
#include<thread>
#include<Windows.h>
using namespace std;

#define Enter	13
#define Escape	27

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
	Tank(int volume) :VOLUME
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

#define MAX_SPEED_LOW_LIMIT		 50
#define MAX_SPEED_HIGH_LIMIT	447
class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	struct
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
	}threads;
	int speed;
	const int MAX_SPEED;
	int acceleration;
public:
	Car(int consumption = 10, int volume = 60, int max_speed = 250) :
		engine(consumption),
		tank(volume),
		driver_inside(false),
		speed(0),
		acceleration(MAX_SPEED / 20),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		)
	{
		cout << "Your car is ready to go" << endl;
	}
	~Car()
	{
		cout << "Your car is over" << endl;
	}
	void get_in()
	{
		driver_inside = true;
		threads.panel_thread = std::thread(&Car::panel, this);
	}
	void get_out()
	{
		driver_inside = false;
		if (threads.panel_thread.joinable())threads.panel_thread.join();
		system("CLS");
		cout << "Out of the car" << endl;
	}
	void start()
	{
		if (tank.get_fuel_level())
		{
			engine.start();
			threads.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop()
	{
		engine.stop();
		if (threads.engine_idle_thread.joinable())threads.engine_idle_thread.join();
	}
	void accelerate()
	{
		if (engine.started())
		{
			speed += acceleration;
			if (speed > MAX_SPEED)speed = MAX_SPEED;
			if (!threads.free_wheeling_thread.joinable())
				threads.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
			std::this_thread::sleep_for(1s);
		}
	}
	void slow_down()
	{
		speed -= acceleration;
		if (speed < 0)speed = 0;
		if (threads.free_wheeling_thread.joinable())
			threads.free_wheeling_thread.join();
	}
	void control()
	{
		char key;
		do
		{
			key = _getch();
			switch (key)
			{
			case 'F':case 'f':
				if (driver_inside)
				{
					cout << "Get out of the car\a" << endl;
				}
				else
				{
					double fuel;
					cout << "¬ведите уровень топлива: "; cin >> fuel;
					tank.fill(fuel);
				}
				break;
			case Enter: driver_inside ? get_out() : get_in();	break;
			case 'I':case 'i':	engine.started() ? stop() : start();	break;
			case 'W':case 'w':	accelerate();	break;
			case 'S':case 's':	slow_down();	break;
			case Escape:
				stop();
				get_out();
				break;
			}
		} while (key != 27);
	}
	void free_wheeling()
	{
		while (--speed)
		{
			std::this_thread::sleep_for(1s);
			if (speed < 0)speed = 0;
		}
	}
	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
			this_thread::sleep_for(1s);
	}
	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			cout << "Fuel level:\t" << tank.get_fuel_level() << " liters.\t";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xCF);
				cout << " LOW FUEL ";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl;
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Speed:    " << speed << " km/h\n";
			std::this_thread::sleep_for(100ms);
		}
	}
	void info()const
	{
		engine.info();
		tank.info();
	}
};

//#define TANK_CHECK
//#define (ќпределить)
//директива #define создает макроопределение (макрос)
//#define ENGINE

void main()
{
	setlocale(LC_ALL, "");

#if defined TANK_CHECK
	//если определено TANK_CHECK, то нижеследующий код до диркутивы #endif будет виден компил€тору.
	Tank tank(85);
	int fuel;
	do
	{
		cout << "¬ведите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);
#endif // TANK_CHECK

#ifdef ENGINE
	Engine engine(10);
	engine.info();
#endif // ENGINE

	Car bmw;
	bmw.info();
	bmw.control();
}

/*
------------------------

Pointer
& - Address-of operator (возвращает адрес своего операнда)
* - Dereference operator (возвращает значение по адресу)

Reference

------------------------
*/