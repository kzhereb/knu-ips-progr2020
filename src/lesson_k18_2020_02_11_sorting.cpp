/*
 * lesson_k18-2020_02_11_sorting.cpp
 *
 *  Created on: Feb 11, 2020
 *      Author: KZ
 */
#include <iostream>
#include <algorithm>
#include <map>
#include <cstddef>
#include <ctime>
#include <cassert>

namespace lesson_k18_2020_02_11_sorting {


void insertion_sort_naive(int* array, std::size_t size) {
	for (std::size_t i = 1; i < size; i++) {
		for (std::size_t j = i; j>0 && array[j-1]>array[j];j--) {
			std::swap(array[j],array[j-1]);
		}
	}
}

void insertion_sort_smarter(int* array, std::size_t size) {
	for (std::size_t i = 1; i < size; i++) {
		int temp = array[i];
		std::size_t j;
		for (j = i; j>0 && array[j-1]>temp;j--) {
			array[j] = array[j-1];
		}
		array[j] = temp;
	}
}

void shell_sort(int* array, std::size_t size) {
	static const std::size_t gaps[] = {701, 301, 132, 57, 23, 10, 4, 1 };

	for(auto gap: gaps) {
		int temp;
		for (std::size_t i = gap; i < size; i++) {
			temp = array[i];
			std::size_t j;
			for (j = i; j >= gap && array[j-gap]>temp; j-=gap ) {
				array[j] = array[j-gap];
			}
			array[j] = temp;
		}
	}
}





void merge(int* source, int* destination, std::size_t begin, std::size_t middle, std::size_t end) {
	std::size_t first=begin, second = middle;
	for (std::size_t i = begin; i<end; i++) {
		if ((first<middle) && (second>=end || source[first]<=source[second])) {
			destination[i]=source[first];
			first++;
		} else {
			destination[i]=source[second];
			second++;
		}
	}
}


void merge_sort_twoarrays(int* source, int* destination, std::size_t begin, std::size_t end) {
	if (end-begin<2) {
		return;
	}
	std::size_t middle = (end + begin)/2;
	merge_sort_twoarrays(destination, source,begin,middle);
	merge_sort_twoarrays(destination, source,middle,end);
	merge(source, destination, begin, middle, end);
}

void merge_sort_topdown(int* array, std::size_t size) {
	int * destination = new int[size];
	std::copy(array, array+size, destination);
	merge_sort_twoarrays(destination, array, 0, size);
	delete [] destination;
}

void merge_sort_bottomup(int* array, std::size_t size) {
	int * destination = new int[size];

	for (std::size_t width = 1; width<size; width*=2) {
		for(std::size_t i = 0; i<size; i+=width*2) {
			std::size_t middle = std::min(i+width, size);
			std::size_t end = std::min(i+2*width, size);
			merge(array, destination, i, middle, end);
		}
		std::copy(destination, destination+size,array);
	}
	delete [] destination;
}

std::size_t index_parent(std::size_t i) {
	assert(i>0);
	return (i-1)/2;
}
std::size_t index_left_child(std::size_t i) {
	return 2*i+1;
}
std::size_t index_right_child(std::size_t i) {
	return 2*i+2;
}

void sift_down(int* array, std::size_t start, std::size_t end) {
	std::size_t root = start;
	while (index_left_child(root)<=end) {
		std::size_t child = index_left_child(root);
		std::size_t temp = root;

		if (array[temp]<array[child]) {
			temp = child;
		}
		if (child+1<=end && array[temp]<array[child+1]) {
			temp = child+1;
		}
		if (temp == root) {
			return;
		} else {
			std::swap(array[root],array[temp]);
			root = temp;
		}
	}
}

void heapify(int* array, std::size_t size) {
	for (std::size_t start = index_parent(size-1); ; start--) {
		sift_down(array, start, size-1);
		if (start == 0) { return; }
	}

}

void heap_sort(int* array, std::size_t size) {
	heapify(array,size);
	for(std::size_t end = size-1; end>0; end--) {
		std::swap(array[end],array[0]);
		sift_down(array,0, end-1);
	}
}

void counting_sort(int* array, std::size_t size){
	std::size_t distinct_count = 0;
	std::map<int,std::size_t> key_indices;
	for(std::size_t i = 0; i<size; i++) {
		int key = array[i];
		if (key_indices.count(key) == 0) { //doesn't contain key
			key_indices[key] = distinct_count;
			distinct_count++;
		}
	}
	std::size_t * counts = new std::size_t[distinct_count];
	for (std::size_t i =0; i<distinct_count; i++) {
		counts[i]=0;
	}
	for(std::size_t i = 0; i<size; i++) {
		std::size_t index = key_indices[array[i]];
		counts[index]++;
	}
	std::size_t total = 0;
	for (std::size_t i = 0; i<distinct_count; i++) {
		std::size_t new_total = total+counts[i];
		counts[i] = total;
		total = new_total;
	}

	int* output_array = new int[size];
	for(std::size_t i = 0; i<size; i++) {
		std::size_t index = key_indices[array[i]];
		output_array[counts[index]]= array[i];
		counts[index]++;
	}

	for(std::size_t i = 0; i<size; i++) {
		array[i] = output_array[i];
	}
}


void test_memory_leaks() {
	int test_array[] = {2,74,1,-43, 23, 123, 55, -4, 0, 1, -12345};
	std::size_t size = sizeof(test_array)/sizeof(test_array[0]);
	while (true) {
		merge_sort_topdown(test_array, size);
	}
}

void compare_merge_sorts(std::size_t size) {
	int* array = new int[size];
	for(std::size_t i =0; i<size; i++) {
		array[i] = rand();
	}
	int* array_copy = new int[size];
	std::copy(array, array+size,array_copy);

	auto begin = std::clock();
	merge_sort_topdown(array,size);
	auto end = std::clock();
	std::cout<<"topdown: "<<1000.0 * (end-begin) / CLOCKS_PER_SEC<<" ms"<<std::endl;

	begin = std::clock();
	//merge_sort_bottomup(array_copy,size);
	shell_sort(array_copy,size);
	//insertion_sort_smarter(array_copy,size);
	//heap_sort(array_copy, size);
	end = std::clock();
	std::cout<<"shell_sort: "<<1000.0 * (end-begin) / CLOCKS_PER_SEC<<" ms"<<std::endl;
	delete [] array;
	delete [] array_copy;
}


int main() {

	int test_array[] = {2,74,1,-43, 23, 123, 55, -4, 0, 1, -12345};
	std::size_t size = sizeof(test_array)/sizeof(test_array[0]);
	//shell_sort(test_array, size);
	//insertion_sort_smarter(test_array, size);


	//merge_sort_bottomup(test_array, size);
	//heap_sort(test_array, size);
	counting_sort(test_array,size);
	for(auto i: test_array) {
		std::cout<<i<<" ";
	}
	std::cout<<std::endl;

	compare_merge_sorts(1e6);

//	std::cout<<"test for memory leaks"<<std::endl;
//	test_memory_leaks();


	return 0;
}


}
