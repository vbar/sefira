#ifndef shortmap_hh
#define shortmap_hh

#include <assert.h>
#include <memory> // for allocator
#include "bytemap.hh"
#include "twiddle.hh"

template<typename TValue,
	 typename TAllocator = std::allocator<unsigned char> >
class ShortMap : private TAllocator::template rebind<unsigned char>::other
{
private:
    typedef ByteMap<TValue, TAllocator> TLowItem;

    unsigned short min;

    unsigned short max;

    // NULL when this instance is empty, otherwise array of
    // storage_byte_size bitfields
    TWord *mask;

    TLowItem *storage;

public:
    static const unsigned int none = 0xffffffff;

    typedef typename TAllocator::template rebind<unsigned char>::other
        ByteAllocator;

    ShortMap(const ByteAllocator &a = ByteAllocator()):
        ByteAllocator(a),
	min(0xffff),
	max(0),
	mask(0),
	storage(0)
    {
    }

    ~ShortMap()
    {
        if (storage)
	{
	    destroy_storage();
	}
    }

    bool is_empty() const
    {
        return min > max;
    }

    unsigned int get_min() const
    {
        return is_empty() ? none : min;
    }

    unsigned int get_min(TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	assert(storage);
        storage[min / 256].get_min(vp);
	assert(vp);
	return min;
    }

    unsigned int get_max() const
    {
        return is_empty() ? none : max;
    }

    unsigned int get_max(TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	assert(storage);
        storage[max / 256].get_max(vp);
	assert(vp);
	return max;
    }

    // Returns pointer to the stored value, null iff this instance
    // doesn't store key. Caller can change value through the returned
    // pointer.
    TValue *get_value(unsigned short key)
    {
        if (is_empty())
	{
  	    return 0;
	}

	unsigned char high = key / 256;
	if (!has_bit(mask, high))
	{
	    return 0;
	}

        return storage[high].get_value(key % 256);
    }

    unsigned int successor(unsigned short key) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    return (key < min) ? min : none;
	}

	unsigned char high = key / 256;
	unsigned char low = key % 256;

	unsigned short low_succ;
	if (has_bit(mask, high))
	{
	    low_succ = storage[high].successor(low);
	    if (low_succ != TLowItem::none)
	    {
		return 256 * high + low_succ;
	    }
	}

	for (TIndex i = high + 1; i < 256; ++i)
	{
	    if (has_bit(mask, i))
	    {
		low_succ = storage[i].get_min();
		if (low_succ != TLowItem::none)
		{
		    return 256 * i + low_succ;
		}
	    }
	}

	return none;
    }

    unsigned int successor(unsigned short key, TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	if (min == max)
	{
	    if (key < min)
	    {
	        storage[min / 256].get_min(vp);
		assert(vp);
		return min;
	    }
	    else
	    {
		return none;
	    }
	}

	unsigned char high = key / 256;
	unsigned char low = key % 256;

	unsigned short low_succ;
	if (has_bit(mask, high))
	{
	    low_succ = storage[high].successor(low, vp);
	    if (low_succ != TLowItem::none)
	    {
		return 256 * high + low_succ;
	    }
	}

	for (TIndex i = high + 1; i < 256; ++i)
	{
	    if (has_bit(mask, i))
	    {
		low_succ = storage[i].get_min(vp);
		if (low_succ != TLowItem::none)
		{
		    return 256 * i + low_succ;
		}
	    }
	}

	return none;
    }

    unsigned int predecessor(unsigned short key) const
    {
	if (is_empty())
	{
	    return none;
	}

	assert(storage);

	if (min == max)
	{
	    return (key > max) ? max : none;
	}

	unsigned char high = key / 256;
	unsigned char low = key % 256;

	unsigned short low_pred;
	if (has_bit(mask, high))
	{
	    low_pred = storage[high].predecessor(low);
	    if (low_pred != TLowItem::none)
	    {
		return 256 * high + low_pred;
	    }
	}

	for (TIndex i = high; i > 0; --i)
	{
	    if (has_bit(mask, i - 1))
	    {
		low_pred = storage[i - 1].get_max();
		if (low_pred != TLowItem::none)
		{
		    return 256 * (i - 1) + low_pred;
		}
	    }
	}

	return none;
    }

    unsigned int predecessor(unsigned short key, TValue *&vp) const
    {
	if (is_empty())
	{
	    return none;
	}

	assert(storage);

	if (min == max)
	{
	    if (key > max)
	    {
	        storage[max / 256].get_max(vp);
		assert(vp);
		return max;
	    }
	    else
	    {
		return none;
	    }
	}

	unsigned char high = key / 256;
	unsigned char low = key % 256;

	unsigned short low_pred;
	if (has_bit(mask, high))
	{
	    low_pred = storage[high].predecessor(low, vp);
	    if (low_pred != TLowItem::none)
	    {
		return 256 * high + low_pred;
	    }
	}

	for (TIndex i = high; i > 0; --i)
	{
	    if (has_bit(mask, i - 1))
	    {
		low_pred = storage[i - 1].get_max(vp);
		if (low_pred != TLowItem::none)
		{
		    return 256 * (i - 1) + low_pred;
		}
	    }
	}

	return none;
    }

    void insert(unsigned short key, TValue value)
    {
	if (is_empty()) 
	{	  
	    min = max = key;
	    create_storage();
	}
	else
	{
	    if (key < min)
	    {
		min = key;
	    }
	    else if (max < key)
	    {
		max = key;
	    }
	}

	unsigned char high = key / 256;
	if (!has_bit(mask, high))
	{
	    new (storage + high) TLowItem(*this);
	    mask[high / word_bit_size] |= (1u << (high % word_bit_size));
	}

        storage[high].insert(high, value);
    }

    void set(unsigned short key, TValue value)
    {
	if (is_empty()) 
	{	  
	    min = max = key;
	    create_storage();
	}
	else
	{
	    if (key < min)
	    {
		min = key;
	    }
	    else if (max < key)
	    {
		max = key;
	    }
	}

	unsigned char high = key / 256;
	if (!has_bit(mask, high))
	{
	    new (storage + high) TLowItem(*this);
	    mask[high / word_bit_size] |= (1u << (high % word_bit_size));
	}

        storage[high].set(key % 256, value);
    }

    void erase(unsigned short key)
    {
	if (is_empty()) 
	{
	    assert(!storage);
	    return;
	}

	if (min == max)
	{
	    assert(storage);
	    if (key == min)
	    {
		destroy_storage();
		min = 0xffff;
		max = 0;
	    }

	    return;
	}

	if (key == min)
	{
	    min = (unsigned short)successor(min);
	}
	else if (key == max)
	{
	    max = (unsigned short)predecessor(max);
	}

	assert(min <= max);

        storage[key / 256].erase(key % 256);
    }

private:
    void create_storage()
    {
	mask = reinterpret_cast<TWord *>(this->allocate(word_bit_size));
	memset(mask, 0, word_bit_size);
      
	storage = reinterpret_cast<TLowItem *>(
		this->allocate(256 * sizeof(TLowItem)));
    }

    void destroy_storage()
    {
	for (TIndex i = 0; i < 256; ++i)
	{
	    if (has_bit(mask, i))
	    {
	        storage[i].~TLowItem();
	    }
	}

	this->deallocate(reinterpret_cast<unsigned char *>(storage),
			 256 * sizeof(TLowItem));
	storage = 0;

	this->deallocate(reinterpret_cast<unsigned char *>(mask),
			 word_bit_size);
	mask = 0;
    }
};

#endif
