//
// Created by Peer De bakker on 5/18/22.
//

#include "nm.h"
#include <string.h>
typedef int idx_t;

void	swap(t_symbol *symbols[], const idx_t i, const idx_t j) {
	t_symbol	*tmp = symbols[i];

	symbols[i] = symbols[j];
	symbols[j] = tmp;
}

bool	shouldSwap(const t_symbol *symbol, const t_symbol *pivot, unsigned int flags) {
	const int cmp_ret = strcmp(symbol->name, pivot->name);
	const bool ret = (cmp_ret < 0 || (cmp_ret == 0 && symbol->value < pivot->value));

	if (flags & FLAG_REVERSE_SORT) {
		return (!ret);
	}
	return (ret);
}

idx_t	partition (t_symbol *symbols[], const idx_t low, const idx_t high, const unsigned int flags) {
	t_symbol *pivot = symbols[high];
	idx_t i = low - 1; // Index of smaller element and indicates the right position of pivot found so far

	for (idx_t j = low; j <= high - 1; j++) {
		// if current element is smaller than the pivot
		if (shouldSwap(symbols[j], pivot, flags)) {
			++i;
			swap(symbols, i, j);
		}
	}
	swap(symbols, i + 1, high);
	return (i + 1);
}

void quickSort(t_symbol *symbols[], idx_t low, idx_t high, const unsigned int flags) {
	if (low < high) {
		idx_t partitionIndex = partition(symbols, low, high, flags);

		quickSort(symbols, low, partitionIndex - 1, flags);
		quickSort(symbols, partitionIndex + 1, high, flags);
	}
}

void	sort_symbols(t_symbol *symbols[], const size_t symbol_amount, const unsigned int flags) {
	if (flags & FLAG_NO_SORT) {
		return ;
	}
	quickSort(symbols, 0, (idx_t)(symbol_amount - 1), flags);
}
