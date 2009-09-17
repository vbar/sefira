#ifndef plastic_postorder_hh
#define plastic_postorder_hh

#include <libxml/tree.h>
#include "envelope.hh"
#include "postorderletter.hh"

namespace plastic {

class PostOrder : private Envelope<PostOrderLetter>
{
public:
    PostOrder();
    PostOrder(xmlNodePtr sentinel);
    PostOrder(const PostOrder &other);

private:
    PostOrder(PostOrderLetter *letter);

public:
    xmlNodePtr operator*() const
    {
        return get_letter()->get_current();
    }  

    bool operator==(const PostOrder &other) const
    {
        return get_letter()->get_current() ==
	    other.get_letter()->get_current();
    }

    bool operator!=(const PostOrder &other) const
    {
        return !(*this == other);
    }

    void operator++();

private:
    void ensure_single();
};

}

#endif
