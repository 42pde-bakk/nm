//
// Created by Peer De bakker on 5/18/22.
//

#include "nm.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>

void	assignFirstToSecond(const t_symbol *first, t_symbol *second) {
	second->name = first->name;
	second->type = first->type;
	second->bind = first->bind;
	second->shndx = first->shndx;
	second->value = first->value;
	second->letter = first->letter;
}

void swap(t_symbol *a, t_symbol *b) {
	const t_symbol t = {
			.name = a->name,
			.type = a->type,
			.bind = a->bind,
			.shndx = a->shndx,
			.value = a->value,
			.letter = a->letter
	};

	assignFirstToSecond(b, a);
	assignFirstToSecond(&t, b);
}


/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
//int partition(t_symbol *symbols, int low, int high)
//{
//	t_symbol pivot = symbols[high]; // pivot
//	int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far
//
//	for (int j = low; j <= high - 1; j++)
//	{
//		int diff = strcmp(symbols[i].name, pivot.name);
//		// If current element is smaller than the pivot
////		if (symbols[j] < pivot)
//		if (diff < 0 || (diff == 0 && symbols[i].value < pivot.value))
//		{
//			i++; // increment index of smaller element
//			swap(&symbols[i], &symbols[j]);
//		}
//	}
//	swap(&symbols[i + 1], &symbols[high]);
//	return (i + 1);
//}
//
//void	quicksortIterative(t_symbol *symbols, const size_t n_elems) {
//	t_symbol	stack[n_elems];
//	int top;
//
//	assignFirstToSecond(&symbols[0], &stack[0]);
//	assignFirstToSecond(&symbols[n_elems - 1], &stack[1]);
//	top = 1;
//
//	while (top >= 0) {
//		t_symbol	h,
//					l;
//
//		assignFirstToSecond(&stack[top], &h);
//		top--;
//		assignFirstToSecond(&stack[top], &l);
//		top--;
//
//		// set pivot element at its correct position in sorted array
//	}
//	if (low < high) {
//		/* pi is partitioning index, arr[p] is now at right place */
//		pi = partition(symbols, low, high);
//
//		// Separately sort elements before
//		// partition and after partition
//		quicksort(symbols, n_elems, low, pi - 1);
//		quicksort(symbols, n_elems, pi + 1, high);
//	}
//}
void	swapPointers(t_symbol *left, t_symbol *right) {
//	printf("BEFORE: left=%p, right=%p\n", (void *)left, (void *)right);
	t_symbol *tmp = left;

	left = right;
	right = tmp;
//	printf("AFTER: left=%p, right=%p\n", (void *)left, (void *)right);
}

void swapShit(t_symbol *symbols[], const size_t i) {
	t_symbol *tmp = symbols[i];

	symbols[i] = symbols[i + 1];
	symbols[i + 1] = tmp;
}

void	bubbleSort(t_symbol *symbols[], const size_t n_elems) {
	bool	didSomething = true;

	while (didSomething) {
		didSomething = false;
		for (size_t i = 0; i < n_elems - 1; i++) {
			int ret = strcmp(symbols[i]->name, symbols[i + 1]->name);
			if (ret > 0) { // || (ret == 0 && symbols[i]->value < symbols[i + 1]->value)) {
				dprintf(2, "swapping %s and %s\n", symbols[i]->name, symbols[i+1]->name);
				swapShit(symbols, i);
//				swapPointers(symbols[i], symbols[i + 1]);
				didSomething = true;
			}
		}
		dprintf(2, "went through a loop\n");
	}
}
