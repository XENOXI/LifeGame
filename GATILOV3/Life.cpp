#include "Life.h"
#include <iostream>
#include <locale>
#include <sstream>
#include <time.h>
bool field::get_cell(int i,int j)
{
	if (i < 0)
		i = size_x - 1;
	if (j < 0)
		j = size_y - 1;
	if (i > size_x - 1)
		i = 0;
	if (j > size_y - 1)
		j = 0;
	return Field[i][j];
	return false;
}

void field::gen()
{
	srand(time(0));
	std::vector<int> a;
	std::vector<int> reb={3}, lif={2,3};
	*this = field(SIZE, SIZE, 0, 0, a, a, reb, lif);
	for (int i = 0; i < size_x; i++)
		for (int j = 0; j < size_y; j++)
			Field[i][j] = (rand() % 2 == 0);
}

field::field()
{

}

field::field(int max_x,int max_y,int min_x,int min_y,std::vector<int> x, std::vector<int> y, std::vector<int> reb, std::vector<int> lif)
{
	reborn_rule = reb;
	life_rule = lif;
	size_x = max_x - min_x+9;
	size_y = max_y - min_y+9;
	
	Field = new bool* [size_x];
	SubField = new bool* [size_x];
	for (int i = 0; i < size_x; i++)
	{
		Field[i] = new bool[size_y];
		SubField[i] = new bool[size_y];
		for (int j = 0; j < size_y; j++)
		{
			Field[i][j] = false;
		}
	}

	for (int i =0;i<x.size();i++)
			Field[x[i]-min_x+4][y[i]-min_y+4] = true;
}



void field::step(int step_cnt) {
	iter_cnt += step_cnt;
	for (int w = 0; w < step_cnt; w++)
	{

		for (int i = 0; i < size_x; i++)
		{
			for (int j = 0; j < size_y; j++)
			{
				int cnt = 0;
				for (int x = i - 1; x < i + 2; x++)
					for (int y = j - 1; y < j + 2; y++)
						cnt += get_cell(x, y);
				cnt -= get_cell(i, j);
				SubField[i][j] = false;
				if (Field[i][j])
				{
					for (int k : life_rule)
						if (k == cnt)
						{
							SubField[i][j] = true;
							break;
						}
					continue;
				}
				for (int k : reborn_rule)
					if (k == cnt)
					{
						SubField[i][j] = true;
						break;
					}

			}
		}
		bool** buff = Field;
		Field = SubField;
		SubField = buff;
	}
}


void field::print()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "Правила перехода: B";
	for (int i : reborn_rule)
		std::cout << i;
	std::cout << "/S";
	for (int i : life_rule)
		std::cout << i;
	std::cout << "\nИтерация:" << iter_cnt << '\n';
	setlocale(LC_ALL, "C");
	char v_e = -70; 
	char block = -37;
	char up_left_corner = -55;
	char down_left_corner = -56;
	char up_right_corner = -69;
	char down_right_corner = -68;

	std::cout << up_left_corner;
	for (int j = 0; j < size_y; j++)
		std::cout << "==";
	std::cout << up_right_corner;
	std::cout << '\n';
	for (int i = 0; i < size_x; i++)
	{
		std::cout << v_e;
		for (int j = 0; j < size_y; j++)
		{
			if (Field[i][j])
				std::cout << block<<block;
			else
				std::cout << "  ";
		}
		std::cout << v_e;
		std::cout << '\n';
	}

	std::cout << down_left_corner;
	for (int j = 0; j < size_y; j++)
		std::cout << "==";
	std::cout << down_right_corner<<'\n';

}


bool ui::option(std::string str)
{
	std::stringstream buff;
	buff << str;
	buff >> str;
	if (str == "#Life")
	{
		buff >> str;
		if (str != "1.06")
		{
			throw_exp(0);
			return false;
			//table.gen();
		}
		return true;
	}
	if (str == "#R")
	{
		
		buff >> str;
		std::vector<int> numbers;
		bool is_reb;
		if (str[0] == 'B')
			is_reb = true;
		else if (str[0] == 'S')
			is_reb = false;
		else
		{
			throw_exp(0);
			//table.gen();
			return false;
		}
		if (is_reb)
			table.reborn_rule = parser_numbers(&str);
		else
			table.life_rule = parser_numbers(&str);

		if (str[0] != '/')
		{
			throw_exp(0);
			return false;
		}
		if (str[1] == 'B' && is_reb)
		{
			throw_exp(0);
			return false;
		}
		if (str[1] == 'S' && !is_reb)
		{
			throw_exp(0);
			return false;
		}
		if (str[1]!='S' && str[1]!='B')
		{
			throw_exp(0);
			//table.gen();
			return false;
		}

		if (!is_reb)
			table.reborn_rule = parser_numbers(&str);
		else
			table.life_rule = parser_numbers(&str);

		return true;
	}
	if (str == "#M")
	{
		buff >> game_name;
		return true;
	}

	throw_exp(0);
	return false;
}


void ui::throw_exp(int err)
{
	cls = false;
	setlocale(LC_ALL, "Russian");
	switch (err)
	{
	case 0:
		std::cout << "Неверно составлен файл\n";
		table.gen();
		break;
	case 1:
		std::cout << "Файл не найден\n";
		break;
	case 2:
		std::cout << "Неверная команда\nНапишите help для вызова справки\n";
		break;
	case 3:
		std::cout << "Неверный ввод\n";
		break;
	default:
		cls = true;
		break;
	}

}

std::vector<int> ui::parser_numbers(std::string* str)
{
	int i = 0;
	std::vector<int> res;
	while ((*str)[i] < '0' || (*str)[i]>'9')
		*str = str->substr(1);
	while ((*str)[i] >= '0' && (*str)[i] <= '8')
	{
		res.push_back((*str)[0]-'0');
		*str = str->substr(1);
	}
		
	return res;
};

ui::ui(gamemode m, int argc, char* argv[], std::string filename)
{
	mode = m;
	std::string str, out;
	std::stringstream buff;
	switch (m)
	{
	case file:
		open(filename);
		break;
	case random:
		table.gen();
		game_name = "New game";
		break;
	case offline:
		if (argc == 1)
			throw_exp(3);
		open((std::string)argv[1]);

		for (int i = 2; i < argc; i++)
		{
			str = (std::string)argv[i];
			if (str == "-i")
			{			
				if (i + 1 > argc-1)
				{
					throw_exp(3);
					exit(0);
				}
				tick((std::string)argv[i + 1]);

			}
			if (str.substr(0, 12) == "--iterations")
			{
				std::stringstream buff2;
				std::vector <int> res;
				str = str.substr(13);
				tick(str);
			}
			if (str == "-o")
			{
				if (i + 1 > argc-1 || (std::string)argv[i + 1]=="")
				{
					throw_exp(3);
					exit(0);
				}

				out = (std::string)argv[i + 1];
				
			}
			if (str.substr(0, 8) == "--output")
			{
				std::stringstream buff2;
				std::vector <int> res;
				out = str.substr(9);
			}			
		}
		save(out);
		exit(0);
	default:
		break;
	}
	
}

std::string ui::del_space(std::string str)
{
	if (str.size() == 0)
		return "";
	std::string res;
	int i=0;
	while (str[i] == ' ')
		i++;
	for (int j = i; j < str.size(); j++)
		res.push_back(str[j]);
	if (res.size() == 0)
		return res;
	while (res.back() == ' ')
		res.pop_back();
	return res;
}

void ui::open(std::string filename)
{
	std::vector <int> reborn_rule, life_rule;
	std::ifstream file(filename);
	if (!file.is_open())
	{
		throw_exp(1);
		return;
	}
	std::string buff;
	for (int i = 0; i < 3; i++)
	{
		std::getline(file, buff);
		if (!option(buff))
			return;

	}

	std::vector<int> x, y;
	int max_x = 0, max_y = 0;
	int min_x = 0, min_y = 0;
	for (int i = 0; !file.eof(); i++)
	{
		x.push_back(0);
		y.push_back(0);

		file >> x[i] >> y[i];
		max_x = std::max<int>(max_x, x[i]);
		max_y = std::max<int>(max_y, y[i]);
		min_x = std::min<int>(min_x, x[i]);
		min_y = std::min<int>(min_y, y[i]);
	}
	table  = field(max_x, max_y, min_x, min_y, x, y, table.reborn_rule, table.life_rule);
	//table = costyl;
}

void ui::help()
{
	setlocale(LC_ALL, "Russian");
	
	std::cout << "dump <filename> - сохранить вселенную в файл\ntick <n = 1>(и сокращенно t <n = 1>) - рассчитать n(по умолчанию 1) итераций и напечатать результат.\nexit – завершить игру\nhelp – распечатать справку о командах";
	std::cout << '\n';
	cls = false;
}



void ui::work()
{
	if (cls)
	{
		system("cls");
		std::cout << game_name<<'\n';
		table.print();
	}
	else
		cls = true;
		
	std::cout << "> ";
	std::stringstream stream;
	std::string buff;
	std::getline(std::cin, buff);
	stream << buff;
	std::string cmd, arg;
	stream >> cmd;
	std::getline(stream,arg);
	arg = del_space(arg);
	
	if (cmd == "help")
		help();
	else if (cmd == "exit")
		exit(0);
	else if (cmd == "dump")
		save(arg);
	else if (cmd == "tick" || cmd == "t" || cmd == "")
		tick(arg);
	else
		throw_exp(2);
	

	
}

void ui::save(std::string filename)
{
	std::ofstream out(filename);

	out << "#Life 1.06\n";
	out << "#M " << game_name<<'\n';
	out << "#R B";
	for (int i : table.reborn_rule)
		out << i;
	out << "/S";
	for (int i : table.life_rule)
		out << i;
	out << '\n';
	for (int i = 0; i < table.size_x; i++)
		for (int j = 0; j < table.size_y; j++)
			if (table.Field[i][j])
				out << i << ' ' << j << '\n';
	out.close();
				
}

void ui::tick(std::string arg)
{
	
	for (int i =0;i<arg.size();i++)
		if (arg[i] < '0' || arg[0]>'9')
		{
			throw_exp(2);
			return;
		}

	if (arg.size() == 0)
	{
		table.step(1);
		return;
	}
		
	std::stringstream buff;
	buff << arg;
	int cnt=0;
	buff >> cnt;
	table.step(cnt);
}