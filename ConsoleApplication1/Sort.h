#pragma once
#include <iterator>
#include <vector>
#include <string>

template<typename Itr>
using itr_traits = std::iterator_traits<typename std::remove_reference<Itr>::type>;

template<typename T>
class Sort
{
	static std::vector<T>* aux;

public:
	template<typename Itr>
	static void sort(Itr&& begin, Itr&& end, bool stable = false)
	{
		typename itr_traits<Itr>::iterator_category itr_cat;
		sort_dispatcher(std::forward<Itr>(begin), std::forward<Itr>(end), stable, itr_cat);
	}

private:
	template<typename Itr>
	static void sort_dispatcher(Itr&& begin, Itr&& end, bool stable, std::random_access_iterator_tag&)
	{
		if (stable)
		{
			std::cout << "Merge(stable): " << typeid(T).name() << std::endl;

			aux = new std::vector<T>(end - begin);
			typename std::remove_reference<Itr>::type temp_itr = begin;
			for (T& t : *aux)
			{
				t = *temp_itr++;
			}
			merge_sort(begin, end - 1, aux->begin(), aux->end() - 1);
			delete aux;
			aux = nullptr;
		}
		else
		{
			std::cout << "Quick Sort(unstable): " << typeid(T).name() << std::endl;
			quick_sort(begin, end - 1);
		}
	}

	template<typename Itr>
	static void sort_dispatcher(Itr&& begin, Itr&& end, bool stable, std::forward_iterator_tag&)
	{
		std::cout << "Lower iterator category sort: " << std::endl;

		aux = new std::vector<T>();
		for (typename std::remove_reference<Itr>::type temp_itr = begin; temp_itr != end; temp_itr++)
		{
			aux->push_back(*temp_itr);
		}
		sort(aux->begin(), aux->end(), false);

		auto aux_itr = aux->begin();
		for (typename std::remove_reference<Itr>::type temp_itr = begin; temp_itr != end; temp_itr++, aux_itr++)
		{
			*temp_itr = *aux_itr;
		}

		delete aux;
		aux = nullptr;
	}

	

	template<typename Itr1, typename Itr2>
	static void merge_sort(const Itr1& begin, const Itr1& end, const Itr2& aux_begin, const Itr2& aux_end)
	{
		typename itr_traits<Itr1>::difference_type length = end - begin;

		if (length <= 7)
		{
			insertion_sort(begin, end + 1);
			return;
		}

		typename std::remove_reference<Itr1>::type median = begin + (length / 2);
		typename std::remove_reference<Itr2>::type aux_median = aux_begin + (length / 2);

		merge_sort(aux_begin, aux_median, begin, median);
		merge_sort(aux_median + 1, aux_end, median + 1, end);

		typename std::remove_reference<Itr1>::type main_counter = begin;
		for (typename std::remove_reference<Itr2>::type l_counter = aux_begin, r_counter = aux_median + 1;
			l_counter <= aux_median || r_counter <= aux_end;
			++main_counter)
		{
			if (l_counter > aux_median)
				*main_counter = *r_counter++;
			else if (r_counter > aux_end)
				*main_counter = *l_counter++;
			else if (!less(r_counter, l_counter))
				*main_counter = *l_counter++;
			else
				*main_counter = *r_counter++;
		}
	}



	//////////////////////////////////////Dual Pivo//////////////////////////////////
	template<typename Itr, typename Diff_type>
	static void get_medians(const Itr& begin, const Itr& end, Diff_type length)
	{
		int third =  length / 3;
		typename std::remove_reference<Itr>::type median1 = begin + third;
		typename std::remove_reference<Itr>::type median2 = end - third;

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
	static typename itr_traits<Itr>::difference_type dual_pivot_partition(const Itr& pivot1, const Itr& pivot2, Itr&& less_n, Itr&& greater)
	{
		for (typename std::remove_reference<Itr>::type k = less_n; k <= greater; ++k)
		{
			if (less(k, pivot1))
			{
				swap(k, less_n);
				++less_n;
			}
			else if (less(pivot2, k))
			{
				while (k < greater && less(pivot2, greater))
					--greater;

				swap(k, greater);
				--greater;

				if (less(k, pivot1))
				{
					swap(k, less_n);
					++less_n;
				}
			}
		}

		swap(less_n - 1, pivot1);
		swap(greater + 1, pivot2);

		return greater - less_n;
	}


	template<typename Itr>
	static void quick_sort(const Itr& begin, const Itr& end, unsigned int d = 0)
	{
		typename itr_traits<Itr>::difference_type length = end - begin;


		//For range of 2-28 elements using insertion sort
		if (length < 27)
		{
			insertion_sort(begin, end + 1);
			return;
		}

		//Getting medians
		get_medians(begin, end, length);

		//Pivots
		typename itr_traits<Itr>::value_type pivot1 = *begin;
		typename itr_traits<Itr>::value_type pivot2 = *end;

		//Pointers
		typename std::remove_reference<Itr>::type less = begin + 1;
		typename std::remove_reference<Itr>::type greater = end - 1;

		//Sorting
		typename itr_traits<Itr>::difference_type dist = dual_pivot_partition(begin, end, std::forward<Itr>(less), std::forward<Itr>(greater));

		//Sorting subarrays
		
		if(less - begin >= 2)
			quick_sort(begin, less - 2);
		quick_sort(greater + 2, end);

		//For equal elements
		if (dist > length - 13 && pivot1 != pivot2)
		{
			for (typename std::remove_reference<Itr>::type k = less; k <= greater; ++k)
			{
				if (!(*k < pivot1 || pivot1 < *k))
				{
					swap(k, less);
					++less;
				}
				else if (!(*k < pivot2 || pivot2 < *k))
				{
					swap(k, greater);
					--greater;

					if (!(*k < pivot1 || pivot1 < *k))
					{
						swap(k, less);
						++less;
					}
				}
			}
		}

		//Sorting subarray (P1 < --- < P2)
		if (pivot1 < pivot2)
			quick_sort(less, greater);
	}


	template<typename Itr>
	static void insertion_sort(const Itr& begin, const Itr& end)
	{
		if (begin == end)
			return;

		for (typename std::remove_reference<Itr>::type outer = begin + 1; outer < end; ++outer)
		{
			for (typename std::remove_reference<Itr>::type inner = outer;; --inner)
			{
				typename std::remove_reference<Itr>::type prev_inner = inner - 1;

				if (less(inner, prev_inner))
					swap(prev_inner, inner);

				if (prev_inner == begin)
					break;
			}
		}
	}


	////////////////////////////Assisting func///////////////////////////////////////
	template<typename Itr>
	static bool less(const Itr& a, const Itr& b)
	{
		return *a < *b;
	}


	template<typename Itr>
	static void swap(const Itr& a, const Itr& b)
	{
		typename itr_traits<Itr>::value_type temp = *a;
		*a = *b;
		*b = temp;
	}

	template<typename Type>
	static int get_char(const Type& a, int n);

};

template<typename T>
std::vector<T>* Sort<T>::aux = nullptr;

template<>
template<typename Itr>
bool Sort<std::string>::less(const Itr& a, const Itr& b)
{
	int length = std::min((*a).length(), (*b).length());
	for (int c = 0; c < length; c++)
	{
		if (tolower((*a)[c]) < tolower((*b)[c]))
			return true;
		if (tolower((*a)[c]) > tolower((*b)[c]))
			return false;
	}
}

template<>
template<>
int Sort<std::string>::get_char(const std::string & a, int n)
{
	if (n < a.length())
		return a[n];
	else
		return -1;
}

template<>
template<typename Itr>
void Sort<std::string>::quick_sort(const Itr& begin, const Itr& end, unsigned int d)
{
	if (end - begin <= 5)
	{
		insertion_sort(end, begin);
		return;
	}

	typename std::remove_reference<Itr>::type l_itr = begin;
	typename std::remove_reference<Itr>::type g_itr = end;
	typename std::remove_reference<Itr>::type i = begin + 1;

	int pivot = get_char(*begin, d);


	while (i <= g_itr)
	{
		int c = get_char(*i, d);

		if (c < pivot)
			swap(l_itr++, i++);
		else if (pivot < c)
			swap(i, g_itr--);
		else
			i++;
	}

	quick_sort(begin, l_itr - 1, d);
	if (pivot >= 0) quick_sort(l_itr, g_itr, d + 1);
	quick_sort(g_itr + 1, end, d);

}