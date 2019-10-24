#include "Sort.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <utility> //std::pair
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm> //C++ quick sort
#include <iterator>
#include <deque>

template<typename T>
void my_swap(T& elem1, T& elem2)
{
	T temp_data{};
	temp_data = elem1;
	elem1 = elem2;
	elem2 = temp_data;
}


template<typename T>
void shuffle(std::vector <T>& vec)
{
	for (size_t i{ 0 }; i < vec.size(); i++)
	{
		my_swap(vec.at(i), vec.at(rand() % (vec.size() - i) + i));
	}
}

class A
{
public:
	int age;
	std::string name;
	A(int a, std::string n = "Empty")
	{
		age = a;
		name = n;
	}

	A() : A(15) {};
	bool operator<(const A& obj2)
	{
		return age < obj2.age;
	}

	bool operator!=(const A& obj2)
	{
		return age != obj2.age;
	}
};

template<typename T, template<class _Ty, class _Alloc = std::allocator<_Ty>> typename Container>
void fill_containers(Container<T>& container1, Container<T>& container2, Container<T>& container3, int size)
{
	for (int i = 1; i < size; i++)
	{
		T temp(rand() * 3 % (rand() % (i + 5) + 1));
		container1.push_back(temp);
		container2.push_back(temp);
		container3.push_back(temp);
	}
}

////template<template<class _Ty, class _Alloc = std::allocator<_Ty>> typename Container>
////void fill_containers<std::string, Container>(Container<std::string>& container1, Container<std::string>& container2, int size)
////{
////	for (int i = 1; i < size; i++)
////	{
////		int size = (rand() * 3 % (rand() % (i + 5) + 1)) % 100;
////		std::string temp(size);
////		for (int i = 0; i < size; i++)
////			temp[i] = (rand() % 96);
////		container1.push_back(temp);
////		container1.push_back(temp);
////	}
////}

template<typename T, template<class _Ty, class _Alloc = std::allocator<_Ty>> typename Container>
void test_func(int size, bool stable = false) 
{
	clock_t sortstart{};

	Container<T> container1;
	Container<T> container2;
	Container<T> container3;

	fill_containers<T>(container1, container2, container3, size);

	std::sort(container1.begin(), container1.end());

	std::cout << "rvalue" << std::endl;
	sortstart = clock();
	Sort<T>::sort(container2.begin(), container2.end(), stable);
	std::cout << "Time taken(my sort): " << (double)(clock() - sortstart) / CLOCKS_PER_SEC << std::endl;

	auto beg_itr1 = container1.begin();
	auto beg_itr2 = container2.begin();

	auto end_itr1 = container1.end();
	auto end_itr2 = container2.end();

	for (; beg_itr1 != end_itr1 && beg_itr2 != end_itr2; ++beg_itr1, ++beg_itr2)
	{
		if (*beg_itr1 != *beg_itr2)
		{
			std::cout << "Sort Error" << std::endl;
			return;
		}
	}
	std::cout << "Succes!" << std::endl;

	std::cout << "lvalue" << std::endl;

	beg_itr1 = container1.begin();
	beg_itr2 = container3.begin();

	end_itr1 = container1.end();
	end_itr2 = container3.end();

	sortstart = clock();
	Sort<T>::sort(beg_itr2, end_itr2, stable);
	std::cout << "Time taken(my sort): " << (double)(clock() - sortstart) / CLOCKS_PER_SEC << std::endl;

	for (; beg_itr1 != end_itr1 && beg_itr2 != end_itr2; ++beg_itr1, ++beg_itr2)
	{
		if (*beg_itr1 != *beg_itr2)
		{
			std::cout << "Sort Error" << std::endl;
			return;
		}
	}
	std::cout << "Succes!" << std::endl;
}


int main()
{
	//test_func<int, std::vector>(1000000, true);

	//test_func<double, std::deque>(1000000);

	//test_func<std::string, std::vector>(1000000);

	test_func<A, std::vector>(100000, true);
	test_func<A, std::vector>(100000, false);
	return 0;
}

