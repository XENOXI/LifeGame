#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#define SIZE 20



class field
{
private:
	bool** SubField=NULL;

	bool get_cell(int,int);
public:
	void gen();
	unsigned int size_x;
	unsigned int size_y;
	bool** Field=NULL;
	std::vector<int> reborn_rule;
	std::vector<int> life_rule;
	field();
	~field();
	field(int , int , int , int , std::vector<int> , std::vector<int>, std::vector<int>, std::vector<int>);
	void step(int);
	void print();
};

enum gamemode
{
	file,random,offline
};



class ui
{
private:
	field table;
	std::string del_space(std::string);
	void option(std::string);
	void throw_exp(int);
	std::vector<int> parser_numbers(std::string*);
	std::string game_name;
	bool cls=true;
	gamemode mode;
public:

	ui(gamemode,int ,char*[], std::string filename = "");
	void open(std::string);
	void help();
	void work();
	void save(std::string);
	void tick(std::string);
};