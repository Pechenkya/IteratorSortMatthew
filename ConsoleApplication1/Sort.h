#pragma once
#include <iterator>
#include <vector>


//namespace std
//{
//	template< class T >
//	struct allocator;
//
//	template<
//		class T,
//		class Allocator = std::allocator<T>
//	> class vector;
//}

template<typename T>
class Sort
{
public:
	template<typename Itr>
	static void sort(Itr&& begin, Itr&& end, bool stable)
	{

	}


	template<typename Itr>
	static void merge_sort(Itr&& begin, Itr&& end, std::vector<T>& aux_vec)
	{

	}


	template<typename Itr, typename Diff_type>
	static void get_medians(Itr&& begin, Itr&& end, Diff_type length)
	{
		int third =  length / 3;
		Itr median1 = begin + third;
		Itr median2 = end - third;

		if (median1 <= begin)
			median1 = begin + 1;

		if (median2 >= end)
			median2 = end - 1;

		if (less(median1, median2))
		{
			swap(median1, begin);
			swap(median2, end);
		}
		else
		{
			swap(median1, end);
			swap(median2, begin);
		}
	}


	template<typename Itr>
	static int dual_pivot_partition(Itr&& pivot1, Itr&& pivot2, Itr&& lesst, Itr&& greater)
	{
		for (Itr k = lesst; k <= greater; ++k)
		{
			if (less(k, pivot1))
			{
				swap(k, lesst);
				++lesst;
			}
			else if (less(pivot2, k))
			{
				while (k < greater && less(pivot2, greater))
					--greater;

				swap(k, greater);
				--greater;

				if (less(k, pivot1))
				{
					swap(k, lesst);
					++lesst;
				}
			}
		}

		swap(lesst--, pivot1);
		swap(greater++, pivot2);

		return greater - lesst;
	}


	template<typename Itr>
	static void dual_pivot(Itr&& begin, Itr&& end)
	{
		//typedef typename std::iterator_traits<Itr>::difference_type diff;
		//typedef typename std::iterator_traits<Itr>::value_type val;

		typename std::iterator_traits<Itr>::difference_type length = end - begin;

		//For range of 1-28 elements using insertion sort
		//if (length < 27)
		//{
		//	insertion_sort(begin, end);
		//	return;
		//}
		if (length < 1)
		{
			return;
		}

		//Getting medians
		get_medians(std::forward(begin), std::forward(end), length);

		//Pivots
		typename std::iterator_traits<Itr>::value_type pivot1 = *begin;
		typename std::iterator_traits<Itr>::value_type pivot2 = *end;

		//Pointers
		Itr less = begin++;
		Itr greater = end--;

		//Sorting
		typename std::iterator_traits<Itr>::difference_type dist = dual_pivot_partition(std::forward(begin), std::forward(end), std::forward(less), std::forward(greater));

		//Sorting subarrays
		dual_pivot(std::forward(begin), std::forward(less - 2));
		dual_pivot(std::forward(greater + 2), std::forward(end));

		//For equal elements
		if (dist > length - 13 && pivot1 != pivot2)
		{
			for (Itr k = less; k <= greater; ++k)
			{
				if (*k == pivot1)
				{
					swap(std::forward(k), std::forward(less));
					++less;
				}
				else if (*k == pivot2)
				{
					swap(std::forward(k), std::forward(greater));
					--greater;

					if (*k == pivot1)
					{
						swap(std::forward(k), std::forward(less));
						++less;
					}
				}
			}
		}

		//Sorting subarray (P1 < --- < P2)
		if (pivot1 < pivot2)
			dual_pivot(std::forward(less), std::forward(greater));
	}


	template<typename Itr>
	static void insertion_sort(Itr&& begin, Itr&& end)
	{
		for (Itr outer = begin + 1; outer != end; ++outer)
		{
			Itr inner = outer;
			for (typename std::iterator_traits<Itr>::difference_type i = outer - begin; i > 0 && less(outer, outer - i); i--, --inner);

			swap(outer, inner);
		}
	}


	template<typename Itr>
	static bool less(Itr&& a, Itr&& b)
	{
		return *a < *b;
	}


	template<typename Itr>
	static void swap(Itr&& a, Itr&& b)
	{
		typename std::iterator_traits<Itr>::value_type temp = *a;
		*a = *b;
		*b = temp;
	}


};



