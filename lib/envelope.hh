#ifndef envelope_hh
#define envelope_hh

#include <algorithm>

template<typename TPayload>
class Envelope
{
public:
    typedef TPayload TLetter;

private:
    TPayload *payload;
    int *refCnt;

protected:
    Envelope():
        payload(new TPayload()),
	refCnt(new int(1))
    {
    }

    Envelope(TPayload *p):
        payload(p),
	refCnt(new int(1))
    {
    }

    Envelope(const Envelope &other):
	payload(other.payload),
	refCnt(other.refCnt)
    {
	add_ref();
    }

    ~Envelope()
    {
        del_ref();
    }

    int get_ref_cnt() const { return *refCnt; }

    Envelope &operator=(const Envelope &other)
    {
	other.add_ref();
	del_ref();

	payload = other.payload;
	refCnt = other.refCnt;

	return *this;
    }

    bool is_shared() {
        return *refCnt > 1;
    }

    TPayload *get_letter() {
        return payload;
    }

    const TPayload *get_letter() const {
        return payload;
    }

    void swap(Envelope &other) {
        std::swap(payload, other.payload);
	std::swap(refCnt, other.refCnt);
    }

private:
    void add_ref() const { ++*refCnt; }

    void del_ref()
    {
	if (!(--*refCnt)) {
	    delete payload;
	    delete refCnt;
	}
    }
};

#endif
