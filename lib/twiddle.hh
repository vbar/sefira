#ifndef twiddle_hh
#define twiddle_hh

typedef unsigned int TWord;

typedef unsigned int TIndex;

static const unsigned short nobyte = 0xffff;

static const TIndex word_bit_size = 8 * sizeof(TWord);

static const TIndex storage_byte_size = 32 / sizeof(TWord);

unsigned short get_lowest(TWord word,
			  TIndex start_bit);

unsigned short get_highest(TWord word,
			   TIndex start_bit);

// bits must not be NULL
inline bool has_bit(TWord *bits, unsigned char x)
{
    return bits[x / word_bit_size] & (1u << (x % word_bit_size));
}

#endif
