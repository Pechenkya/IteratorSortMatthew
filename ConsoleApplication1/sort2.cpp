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

template<typename Hui>
void get_type(Hui&& elem)
{
	std::cout << std::is_lvalue_reference<Hui>::value << 7 - elem << std::endl;
}

template<typename Hui>
void func2(const Hui& elem)
{
	get_type(elem);
}


template<typename Hui>
void func1(Hui&& elem) 
{
	func2(elem);
	func2(std::forward<Hui>(elem));
}




int main()
{
	clock_t sortstart{};

	std::ifstream inw("Words.txt");
	//std::ofstream outw("Sorted words.txt");
	std::ofstream outw("Sorted vector.txt");

	int size = 10000000;

	//std::list<int> list;

	std::vector <int> vector_elements(size);
	//std::vector <std::string> vector_elements{ "Back", "Hall", "Anime", "Counter-Strike", "Horisontal", "GGWP", "School", "Piramide", "Hall", "Matthew-Gay" };

	for (int i = 1; i < size; i++)
	{
		//list.push_back(rand()*3 % (rand() % (i+5) + 1));
		//vector_elements.push_back(rand() % i);
		vector_elements.at(i - 1) = rand() * 3 % (rand() % (i + 5) + 1);
		//vector_elements.at(i - 1) = 0;
		//vector_elements.at(i - 1) = rand() % 2;
	}

	//for (int i = 0; i < 13008; i++)
	//{
	//	std::string temp{};
	//	inw >> temp;
	//	vector_elements.push_back(temp);
	//}


	//shuffle(vector_elements);


	std::cout << "Started sort(my sort)" << std::endl;
	sortstart = clock();
	Sort<int>::sort(vector_elements.begin(), vector_elements.end(), false);
	std::cout << "Time taken(my sort): " << (double)(clock() - sortstart) / CLOCKS_PER_SEC << std::endl;


	for (auto t : vector_elements)
		outw << t << std::endl;

	return 0;
}

