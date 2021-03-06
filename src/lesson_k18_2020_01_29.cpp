/*
 * lesson_k18_2020_01_29.cpp
 *
 *  Created on: Jan 29, 2020
 *      Author: KZ
 */
#include <iostream>

namespace lesson_k18_2020_01_29 {
struct ListNode {
	int value;
	ListNode* prev;
	ListNode* next;
	ListNode(int value) {
		this->value = value;
		prev = next = nullptr;
	}
};

struct List {
	ListNode* head;
	ListNode* tail;
	std::size_t length;

	List() {
		head = nullptr;
		tail = nullptr;
		length = 0;
	}
	void push_back(int value) {
		ListNode * node = new ListNode(value);
		node->prev = tail;
		if (tail) {
			tail->next = node;
		}
		tail = node;
		if ( ! head) {
			head = node;
		}
		length++;
	}
};

std::ostream& operator<<(std::ostream& out, const List& list) {
	out<<"List of length "<<list.length<<std::endl;
	ListNode* cur = list.head;
	std::size_t i = 0;
	while (cur) {
		out<<"  at index "<<i<<": item "<<cur->value<<std::endl;
		cur = cur->next;
		i++;
	}
	return out;
}


int main() {
	List my_list;
	my_list.push_back(228);
	my_list.push_back(322);
	std::cout<<"List head: "<<my_list.head->value
			<<"List tail: "<<my_list.tail->value<<std::endl;
	std::cout<<my_list;
	return 0;
}

}
