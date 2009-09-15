#include "twiddle.hh"

unsigned short get_lowest(TWord word,
    TIndex start_bit)
{
    if (!word)
    {
	return nobyte;
    }

    TWord mask = 1u << start_bit;
    for (TIndex i = start_bit; i < word_bit_size; ++i)
    {
	if (word & mask)
	{
	    return i;
	}

	mask *= 2;
    }

    return nobyte;
}

unsigned short get_highest(TWord word,
    TIndex start_bit)
{
    if (!word)
    {
	return nobyte;
    }

    TWord mask = 1u << start_bit;
    for (TIndex i = start_bit + 1; i > 0; --i)
    {
	if (word & mask)
	{
	    return i - 1;
	}

	mask /= 2;
    }

    return nobyte;
}
