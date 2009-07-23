#ifndef optimistic_byteset_hh
#define optimistic_byteset_hh

#include <string.h>
#include <assert.h>
#include <memory> // for allocator
#include "twiddle.hh"

namespace optimistic {

template<typename TAllocator = std::allocator<unsigned char> >
class ByteSet : private TAllocator::template rebind<unsigned char>::other
{
protected:
    // 0xff when this instance is empty, otherwise its minimal key
    unsigned char min;

    // 0 when this instance is empty, otherwise its maximal key
    unsigned char max;

    // NULL when this instance has at most 1 key, otherwise array of
    // storage_byte_size bitfields
    TWord *mask;

public:
    typedef typename TAllocator::template rebind<unsigned char>::other
        ByteAllocator;

    static const unsigned short none = nobyte;

    ByteSet(const ByteAllocator &a = ByteAllocator()): // doesn't throw
        ByteAllocator(a)
    {
	min = 0xff;
	max = 0;
	mask = 0;
    }

    ~ByteSet()
    {
        if (mask)
	{
	    this->deallocate(reinterpret_cast<unsigned char *>(mask),
			     storage_byte_size);
	}
    }

    bool is_empty() const
    {
        return min > max;
    }

    unsigned short get_min() const
    {
        return is_empty() ? none : min;
    }

    unsigned short get_max() const
    {
        return is_empty() ? none : max;
    }

    bool has_value(unsigned char key) const
    {
        if (is_empty())
	{
  	    return false;
	}

	if (min == key)
  	{
	    return true;
	}

	if (!mask)
	{
	    return false;
	}

        return has_bit(mask, key);
    }

    unsigned short successor(unsigned char key) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    return (key < min) ? min : none;
	}

	assert(mask);

	TIndex start_word = key / word_bit_size;
	TIndex start_bit = key % word_bit_size;
	TIndex lowest;

	if (start_bit < word_bit_size - 1)
	{
	    lowest = get_lowest(mask[start_word],
				start_bit + 1);
	    if (lowest != none)
	    {
		return word_bit_size * start_word + lowest;
	    }
	}

	for (TIndex b = start_word + 1; b < storage_byte_size; ++b)
	{
	    lowest = get_lowest(mask[b], 0);
	    if (lowest != none)
	    {
		return word_bit_size * b + lowest;
	    }
	}

	return none;
    }

    unsigned short predecessor(unsigned char key) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    return (key > max) ? max : none;
	}

	assert(mask);

	TIndex start_word = key / word_bit_size;
	TIndex start_bit = key % word_bit_size;
	TIndex highest;

	if (start_bit)
	{
	    highest = get_highest(mask[start_word],
				  start_bit - 1);
	    if (highest != none)
	    {
		return word_bit_size * start_word + highest;
	    }
	}

	for (TIndex b = start_word; b > 0; --b)
	{
	    highest = get_highest(mask[b - 1], word_bit_size - 1);
	    if (highest != none)
	    {
		return word_bit_size * (b - 1) + highest;
	    }
	}

	return none;
    }

    void insert(unsigned char key)
    {
	if (is_empty()) 
	{
	    min = max = key;
	}
	else
	{
	    if (!mask)
	    {
		assert(min == max);
		mask = reinterpret_cast<TWord *>(
		    this->allocate(word_bit_size));
		memset(mask, 0, word_bit_size);
		set(min);
	    }

	    set(key);

	    if (key < min)
	    {
		min = key;
	    }
	    else if (max < key)
	    {
		max = key;
	    }
	}
    }

    void erase(unsigned char key)
    {
	if (is_empty()) 
	{
	    assert(!mask);
	    return;
	}

	if (min == max)
	{
	    assert(!mask);
	    if (key == min)
	    {
		min = 0xff;
		max = 0;
	    }

	    return;
	}

	assert(mask);

	if (key == min)
	{
	    min = (unsigned char)successor(min);
	    assert(min <= max);
	    if (min == max)
	    {
	        this->deallocate(reinterpret_cast<unsigned char *>(mask),
				 storage_byte_size);
		mask = 0;
	    }
	}
	else if (key == max)
	{
	    max = (unsigned char)predecessor(max);
	    assert(min <= max);
	    if (min == max)
	    {
	        this->deallocate(reinterpret_cast<unsigned char *>(mask),
				 storage_byte_size);
		mask = 0;
	    }
	}

	if (mask)
	{
	    unset(key);
	}
    }

protected:
    ByteAllocator *get_allocator()
    {
        return this;
    }

    // assumes mask is initialized
    void set(unsigned char key)
    {
        mask[key / word_bit_size] |= 1u << (key % word_bit_size);
    }

    // assumes mask is initialized
    void unset(unsigned char key)
    {
        mask[key / word_bit_size] &= ~(1u << (key % word_bit_size));
    }
};

}

#endif
