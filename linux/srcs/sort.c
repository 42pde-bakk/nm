//
// Created by Peer De bakker on 5/18/22.
//

#include "nm.h"
#include <string.h>

void	swap(t_symbol *symbols[], const idx_t i, const idx_t j) {
	t_symbol	*tmp = symbols[i];

	symbols[i] = symbols[j];
	symbols[j] = tmp;
}

idx_t	partition (t_symbol *symbols[], idx_t low, idx_t high) {
	t_symbol *pivot = symbols[high];
	idx_t i = low - 1; // Index of smaller element and indicates the right position of pivot found so far

	for (idx_t j = low; j <= high - 1; j++) {
		// if current element is smaller than the pivot
		const int	cmp_ret = strcmp(symbols[j]->name, pivot->name);
		if (cmp_ret < 0 || (cmp_ret == 0 && symbols[j]->value < pivot->value)) {
			++i;
			swap(symbols, i, j);
		}
	}
	swap(symbols, i + 1, high);
	return (i + 1);
}

void	quickSort(t_symbol *symbols[], idx_t low, idx_t high) {
	if (low < high) {
		idx_t partitionIndex = partition(symbols, low, high);

		quickSort(symbols, low, partitionIndex - 1);
		quickSort(symbols, partitionIndex + 1, high);
	}
}
