#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <Windows.h>
#include <conio.h>
using namespace std;
//tasks 16, 25
namespace lab2b
{
	struct Time
	{
		unsigned int hour;
		unsigned int minute;
		unsigned int second;

		Time()
		{
			hour = 0;
			minute = 0;
			second = 0;
		}

		Time(unsigned int hour, unsigned int minute, unsigned int second)
		{
			this->hour = hour;
			this->minute = minute;
			this->second = second;
		}

	};

	struct Journal
	{
		vector<string> data;
		vector<Time> time;
		size_t size = 0;

		Journal(size_t size)
		{
			this->size = size;
		}

		void add_element(string message, Time exact_time)
		{
			if (data.size() < size)
			{
				this->data.push_back(message);
				this->time.push_back(exact_time);
			}
			else
			{
				for (size_t i = 0; i < size - 1; i--)
				{
					data[i] = data[i + 1];
					time[i] = time[i + 1];
				}
				data[size - 1] = message;
				time[size - 1] = exact_time;
			}
		}

		Journal get_k_last_messages(size_t k)
		{
			Journal new_journal(k);
			for (size_t i = 0; i < k; i++)
			{
				new_journal.data[k - i - 1] = data[size - i - 1];
				new_journal.time[k - i - 1] = time[size - i - 1];
			}
			return new_journal;
		}

	};

	
#define MAX_LEVEL 6
	const double p = 0.5; // probability(usually 1/2 or 1/4)

	struct snode
	{
		int value;
		snode** forw;
		snode(int level, int& value)
		{
			forw = new snode * [level + 1];
			memset(forw, 0, sizeof(snode*) * (level + 1));
			this->value = value;
		}
		~snode()
		{
			delete[] forw;
		}
	};
	
	struct SkipList
	{
		snode* header;
		int value;
		int level;
		SkipList()
		{
			header = new snode(MAX_LEVEL, value);
			level = 0;
		}
		~SkipList()
		{
			delete header;
		}
		bool contains_element(int& s_value)
		{
			snode* x = header;
			for (int i = level;i >= 0;i--)
			{
				while (x->forw[i] != NULL && x->forw[i]->value < s_value)
				{
					x = x->forw[i];
				}
			}
			x = x->forw[0];
			return x != NULL && x->value == s_value;
		}
		void add_element(int& value)
		{
			snode* x = header;
			snode* update[MAX_LEVEL + 1];
			memset(update, 0, sizeof(snode*) * (MAX_LEVEL + 1));
			for (int i = level;i >= 0;i--)
			{
				while (x->forw[i] != NULL && x->forw[i]->value < value)
				{
					x = x->forw[i];
				}
				update[i] = x;
			}
			x = x->forw[0];
			if (x == NULL || x->value != value)
			{
				int lvl = random_level();
				if (lvl > level)
				{
					for (int i = level + 1;i <= lvl;i++)
					{
						update[i] = header;
					}
					level = lvl;
				}
				x = new snode(lvl, value);
				for (int i = 0;i <= lvl;i++)
				{
					x->forw[i] = update[i]->forw[i];
					update[i]->forw[i] = x;
				}
			}
		}
	};


	/*
	 * Random Value Generator
	 */
	float frand()
	{
		return (float)rand() / RAND_MAX;
	}

	/*
	 * Random Level Generator
	 */
	int random_level()
	{
		static bool first = true;
		if (first)
		{
			srand((unsigned)time(NULL));
			first = false;
		}
		int lvl = (int)(log(frand()) / log(1. - p));
		return lvl < MAX_LEVEL ? lvl : MAX_LEVEL;
	}




	void test(bool num)
	{
		int r = 0;
		while (true)
		{
			if (num)
			{
				std::cout << "\tструктура\n";
				std::cout << "\t\t1. журнал повідомлень\n";
				std::cout << "\t\t2. лист з пропусками\n";

				r = _getch() - 48;
				if (r == 1)
				{
					std::cout << "\t\t\tвведіть розмір\n";
					size_t size;
					cin >> size;
					Journal a(size);
					while (true)
					{
						std::cout << "\t\t\tнаступна операція\n";
						std::cout << "\t\t\t\t1. додати елемент\n";
						std::cout << "\t\t\t\t2. отримат K останніх повідомлен\n";
						r = _getch() - 48;

						if (r == 1)
						{
							std::cout << "\t\t\t\t\tвведіть текст і час у форматі год/хв/сек\n";
							string mes;
							cin >> mes;
							int h, m, s;
							cin >> h >> m >> s;
							Time t(h, m, s);
							a.add_element(mes, t);
						}
						else if (r == 2)
						{
							std::cout << "\t\t\t\t\tвведіть k\n";
							size_t k;
							cin >> k;
							a.get_k_last_messages(k);
						}
						else
						{
							continue;
						}
					}
				}
				else if (r == 2)
				{
					SkipList a;

					while (true)
					{
						std::cout << "\t\t\tоперація\n";
						std::cout << "\t\t\t\t1 .додати елемент\n";
						std::cout << "\t\t\t\t2. знайти елемент\n";
						r = getch() - 48;
						if (r == 1)
						{
							std::cout << "\t\t\t\t\tвведіть значеня елементу\n";
							int el;
							cin >> el;
							a.add_element(el);
						}
						else if (r == 2)
						{
							std::cout << "\t\t\t\t\tвведіть значеня елементу\n";
							int el;
							cin >> el;
							if (a.contains_element(el))
							{
								std::cout << "\t\t\t\t\tзнайедно\n";
							}
							else
							{
								std::cout << "\t\t\t\t\tне знайедно\n";
							}
						}
						else
						{
							continue;
						}
					}
				}
				else
				{
					continue;
				}
			}
			else
			{

			}
		}

	}

	int main()
	{
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		int r = 0;

		while (1)
		{
			std::cout << "Режими роботи:\n";
			std::cout << "1.	Інтерактивний діалоговий режим\n";
			std::cout << "2.	Демонстраційний режим\n";
			std::cout << "3.	Вихід із программи\n";
			r = _getch() - 48;
			if (r == 1)
			{
				test(true);

			}
			else if (r == 2)
			{
				test(false);
			}
			else if (r == 3)
				break;
			else continue;
		}
		return 0;
	}
}
