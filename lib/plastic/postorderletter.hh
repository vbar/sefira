#ifndef plastic_postorderletter_hh
#define plastic_postorderletter_hh

#include <libxml/tree.h>

namespace plastic
{

class PostOrderLetter
{
private:
    xmlNodePtr sentinel;
    xmlNodePtr current;

public:
    PostOrderLetter();

    PostOrderLetter(xmlNodePtr sentinel);

    PostOrderLetter(xmlNodePtr sentinel, xmlNodePtr current):
        sentinel(sentinel),
	current(current)
    {
    }

    PostOrderLetter *clone() const
    {
        return new PostOrderLetter(sentinel, current);
    }

    xmlNodePtr get_current() const { return current; }

    void inc();

private:
    void descend();
};

}

#endif
