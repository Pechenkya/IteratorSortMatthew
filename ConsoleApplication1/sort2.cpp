#include "Sort.h"

int main()
{
	int arr[100];
	std::vector <int> arr1(100);

	int counter{ 1 };
	for (auto & t : arr1)
	{
		t = rand() % counter++;
	}
	auto iter1 = arr1.begin();
	auto iter2 = arr1.end() - 1;

	Sort<int>::dual_pivot(iter1, iter2);

	return 0;
}

