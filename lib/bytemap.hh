#ifndef bytemap_hh
#define bytemap_hh

#include <string.h>
#include <assert.h>
#include <memory> // for allocator
#include "twiddle.hh"
#include "byteset.hh"

template<typename TValue, typename TAllocator = std::allocator<unsigned char> >
class ByteMap : private ByteSet<TAllocator>
{
private:
    // NULL when this instance is empty, pointer to TValue array when
    // it has 1 value, otherwise array of storage_byte_size pointers
    void *data;

    typedef typename TAllocator::template rebind<unsigned char>::other
        ByteAllocator;

    using ByteSet<TAllocator>::min;
    using ByteSet<TAllocator>::max;
    using ByteSet<TAllocator>::mask;
    using ByteSet<TAllocator>::get_allocator;
    using ByteSet<TAllocator>::set;
    using ByteSet<TAllocator>::unset;

public:
    using ByteSet<TAllocator>::none;

    ByteMap(const ByteAllocator &a = ByteAllocator()): // doesn't throw
        ByteSet<TAllocator>(a)
    {
	data = 0;
    }

    ~ByteMap()
    {
        if (!data)
	{
	    return;
	}

        if (min == max)
	{
	    TValue *stick = reinterpret_cast<TValue *>(data);
	    stick[min % word_bit_size].~TValue();
	    get_allocator()->deallocate(
                reinterpret_cast<unsigned char *>(data),
		sizeof(TValue) * word_bit_size);
	}
	else
	{
	    delete_rack();
	}
    }

    using ByteSet<TAllocator>::is_empty;
    using ByteSet<TAllocator>::get_min;
    using ByteSet<TAllocator>::get_max;
    using ByteSet<TAllocator>::successor;
    using ByteSet<TAllocator>::predecessor;

    // Returns pointer to the stored value, null iff this instance
    // doesn't store key. Caller can change value through the returned
    // pointer (but not delete it).
    TValue *get_value(unsigned char key)
    {
        if (is_empty())
	{
  	    return 0;
	}

	assert(data);

        if (min == max)
	{
	    if (key == min)
	    {
	        TValue *stick = reinterpret_cast<TValue *>(data);
		return stick + (min % word_bit_size);
	    }
	    else
	    {
		return 0;
	    }
	}

	TValue **rack = reinterpret_cast<TValue **>(data);
	TIndex i = key / word_bit_size;
	TIndex j = key % word_bit_size;
	assert(mask);
	TWord flag = 1u << j;
        if (mask[i] & flag)
	{
	    return rack[i] + j;
	}

	return 0;
    }
    
    unsigned short get_min(TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    vp = reinterpret_cast<TValue *>(data) + (min % word_bit_size);
	}
	else
	{
	    TValue **rack = reinterpret_cast<TValue **>(data);
	    vp = rack[min / word_bit_size] + (min % word_bit_size);
	}

	return min;
    }

    unsigned short get_max(TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    vp = reinterpret_cast<TValue *>(data) + (max % word_bit_size);
	}
	else
	{
	    TValue **rack = reinterpret_cast<TValue **>(data);
	    vp = rack[max / word_bit_size] + (max % word_bit_size);
	}

	return max;
    }

    unsigned short successor(unsigned char key, TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    if (key < min)
	    {
	        vp = reinterpret_cast<TValue *>(data) + (min % word_bit_size);
		return min;
	    }
	    else
	    {
		return none;
	    }
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
		TValue **rack = reinterpret_cast<TValue **>(data);
		vp = rack[start_word] + lowest;
		return word_bit_size * start_word + lowest;
	    }
	}

	for (TIndex b = start_word + 1; b < storage_byte_size; ++b)
	{
	    lowest = get_lowest(mask[b], 0);
	    if (lowest != none)
	    {
		TValue **rack = reinterpret_cast<TValue **>(data);
		vp = rack[b] + lowest;
		return word_bit_size * b + lowest;
	    }
	}

	return none;
    }

    unsigned short predecessor(unsigned char key, TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    if (key > max)
	    {
	        vp = reinterpret_cast<TValue *>(data) + (max % word_bit_size);
		return max;
	    }
	    else
	    {
		return none;
	    }
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
		TValue **rack = reinterpret_cast<TValue **>(data);
		vp = rack[start_word] + highest;
		return word_bit_size * start_word + highest;
	    }
	}

	for (TIndex b = start_word; b > 0; --b)
	{
	    highest = get_highest(mask[b - 1], word_bit_size - 1);
	    if (highest != none)
	    {
		TValue **rack = reinterpret_cast<TValue **>(data);
		vp = rack[b - 1] + highest;
		return word_bit_size * (b - 1) + highest;
	    }
	}

	return none;
    }

    void insert(unsigned char key, TValue value)
    {
	if (is_empty()) 
	{
	    insert_first(key, value);
	    return;
	}

	assert(data);

	if (min == max)
	{
	    assert(!mask);
	    if (key != min)
	    {
	        insert_second(key, value);
	    }
	}
	else
	{
	    assert(mask);
	    if (!has_bit(mask, key))
	    {
	        insert_regular(key, value);
	    }
	}

	if (key < min)
	{
	    min = key;
	}
	else if (max < key)
	{
	    max = key;
	}
    }

    void set(unsigned char key, TValue value)
    {
	if (is_empty()) 
	{
	    insert_first(key, value);
	    return;
	}

	assert(data);

	if (min == max)
	{
	    assert(!mask);
	    if (key != min)
	    {
	        insert_second(key, value);
	    }
	    else
	    {
		TValue *stick = reinterpret_cast<TValue *>(data);
		stick[key % word_bit_size] = value;
		return;
	    }
	}
	else
	{
	    assert(mask);
	    if (!has_bit(mask, key))
	    {
	        insert_regular(key, value);
	    }
	    else
	    {
		TValue **rack = reinterpret_cast<TValue **>(data);
		TValue *stick = rack[key / word_bit_size];
		assert(stick);
		stick[key % word_bit_size] = value;
		return;
	    }
	}

	if (key < min)
	{
	    min = key;
	}
	else if (max < key)
	{
	    max = key;
	}
    }

    void erase(unsigned char key)
    {
	if (is_empty()) 
	{
	    assert(!data);
	    assert(!mask);
	    return;
	}

	assert(data);

	if (min == max)
	{
	    assert(!mask);
	    if (key == min)
	    {
	        TValue *stick = reinterpret_cast<TValue *>(data);
		stick[min % word_bit_size].~TValue();
		get_allocator()->deallocate(
		    reinterpret_cast<unsigned char *>(data),
		    sizeof(TValue) * word_bit_size);
		data = 0;
		min = 0xff;
		max = 0;
	    }

	    return;
	}

	if (key == min)
	{
	    min = (unsigned char)successor(min);
	    assert(min <= max);
	    if (min == max)
	    {
	        singlify(key);
		assert(!mask);
	    }
	}
	else if (key == max)
	{
	    max = (unsigned char)predecessor(max);
	    assert(min <= max);
	    if (min == max)
	    {
	        singlify(key);
		assert(!mask);
	    }
	}

	if (mask)
	{
	    assert(data);
	    TValue **rack = reinterpret_cast<TValue **>(data);

	    TIndex i = key / word_bit_size;
	    TIndex j = key % word_bit_size;
	    TWord flag = 1u << j;

	    if (mask[i] & flag)
	    {
		TValue *hit_stick = rack[i];
		assert(hit_stick);
		hit_stick[j].~TValue();
		if (mask[i] == flag)
		{
		    get_allocator()->deallocate(
			reinterpret_cast<unsigned char *>(hit_stick),
			sizeof(TValue) * word_bit_size);
		    rack[i] = 0;
		}

		mask[i] &= ~flag;
	      }
	}
    }

private:
    void insert_first(unsigned char key, const TValue &value)
    {
	assert(!data);
	assert(!mask);
	min = max = key;
	data = get_allocator()->allocate(
	    sizeof(TValue) * word_bit_size);
	try
	{
	    TValue *stick = reinterpret_cast<TValue *>(data);
	    new (stick + (key % word_bit_size)) TValue(value);
	}
	catch (...)
	{
	    get_allocator()->deallocate(
		reinterpret_cast<unsigned char *>(data),
		sizeof(TValue) * word_bit_size);
	    data = 0;
	    throw;
	}
    }

    void insert_second(unsigned char key, const TValue &value)
    {
	TValue *old_stick = reinterpret_cast<TValue *>(data);

	// will be deallocated by dtor in case of further allocation
	// failure
	mask = reinterpret_cast<TWord *>(
	    get_allocator()->allocate(word_bit_size));
	memset(mask, 0, word_bit_size);

	TValue **rack = reinterpret_cast<TValue **>(
	    get_allocator()->allocate(
		sizeof(TValue *) * storage_byte_size));
	memset(rack, 0, sizeof(TValue *) * storage_byte_size);

	data = 0;

	rack[min / word_bit_size] = old_stick;

	void *new_stick = 0;
	TValue *stick_alias;
	if ((min / word_bit_size) != (key / word_bit_size))
	{
	    try
	    {
		new_stick = get_allocator()->allocate(
		    sizeof(TValue) * word_bit_size);
		stick_alias = reinterpret_cast<TValue *>(new_stick);
	    }
	    catch (...)
	    {
		get_allocator()->deallocate(
		    reinterpret_cast<unsigned char *>(rack),
		    sizeof(TValue *) * word_bit_size);

		old_stick[min % word_bit_size].~TValue();
		get_allocator()->deallocate(
		    reinterpret_cast<unsigned char *>(old_stick),
		    sizeof(TValue) * word_bit_size);

		get_allocator()->deallocate(
		    reinterpret_cast<unsigned char *>(new_stick),
		    sizeof(TValue) * word_bit_size);
		throw;
	    }

	    rack[key / word_bit_size] = stick_alias;
	}
	else
	{
	    stick_alias = old_stick;
	}

	try
	{
	    new (stick_alias + (key % word_bit_size)) TValue(value);
	}
	catch (...)
	{
	    get_allocator()->deallocate(
		reinterpret_cast<unsigned char *>(rack),
		sizeof(TValue *) * word_bit_size);

	    old_stick[min % word_bit_size].~TValue();
	    get_allocator()->deallocate(
		reinterpret_cast<unsigned char *>(old_stick),
		sizeof(TValue) * word_bit_size);

	    if (new_stick)
	    {
		get_allocator()->deallocate(
		    reinterpret_cast<unsigned char *>(new_stick),
		    sizeof(TValue) * word_bit_size);
	    }

	    throw;
	}

	data = rack;

	set(min);
	set(key);
    }

    void insert_regular(unsigned char key, const TValue &value)
    {
	TIndex i = key / word_bit_size;	    
	TIndex j = key % word_bit_size;

	TValue **rack = reinterpret_cast<TValue **>(data);
	if (!mask[i])
	{
	    assert(!rack[i]);
	    rack[i] = reinterpret_cast<TValue *>(
		get_allocator()->allocate(
		    sizeof(TValue) * word_bit_size));
	}

	try
	{
	    new (rack[i] + j) TValue(value);
	}
	catch (...)
	{
	    if (!mask[i])
	    {
		get_allocator()->deallocate(
		    reinterpret_cast<unsigned char *>(rack[i]),
		    sizeof(TValue) * word_bit_size);
		rack[i] = 0;
	    }

	    throw;
	}

	set(key);
    }

    void singlify(unsigned char doomed_key)
    {
        assert(min == max);
	assert(data);
	assert(mask);
	
	TValue **rack = reinterpret_cast<TValue **>(data);

	TIndex i = doomed_key / word_bit_size;
	TIndex j = doomed_key % word_bit_size;
	TValue *hit_stick = rack[i];
	assert(hit_stick); // this method can't be called unless
			   // doomed_key is set
	hit_stick[j].~TValue();
	if (mask[i] == (1u << j))
	{
	    get_allocator()->deallocate(
                reinterpret_cast<unsigned char *>(hit_stick),
		sizeof(TValue) * word_bit_size);
	}

	TValue *stick = rack[min / word_bit_size];
	get_allocator()->deallocate(
	    reinterpret_cast<unsigned char *>(rack),
	    sizeof(TValue *) * storage_byte_size);
	data = stick;

	get_allocator()->deallocate(
            reinterpret_cast<unsigned char *>(mask), storage_byte_size);
	mask = 0;
    }

    void delete_rack()
    {
	assert(mask);
        assert(data);
	TValue **rack = reinterpret_cast<TValue **>(data);
	for (TIndex i = 0; i < storage_byte_size; ++i)
	{
	    if (rack[i])
	    {
	        delete_stick(mask[i], rack[i]);
	    }
	}

	get_allocator()->deallocate(
	    reinterpret_cast<unsigned char *>(data),
	    sizeof(TValue *) * storage_byte_size);
	data = 0;
    }

    void delete_stick(TWord field, TValue *stick)
    {
        TWord bit = 1u;
	for (TIndex i = 0; i < word_bit_size; ++i)
	{
	    if (field & bit)
	    {
	        stick[i].~TValue();
	    }

	    bit *= 2;
	}

	get_allocator()->deallocate(
	    reinterpret_cast<unsigned char *>(stick),
	    sizeof(TValue) * word_bit_size);
    }
};

#endif
