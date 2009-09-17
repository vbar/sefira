#include "postorderletter.hh"
#include <stdexcept>

namespace plastic
{

PostOrderLetter::PostOrderLetter():
    sentinel(0),
    current(0)
{
}

PostOrderLetter::PostOrderLetter(xmlNodePtr sentinel):
    sentinel(sentinel),
    current(sentinel)
{
    descend();
}

void PostOrderLetter::inc()
{
    if (!current)
    {
	throw std::logic_error("iterator past end");
    }

    if (current == sentinel)
    {
	current = 0;
    }
    else
    {
	if (current->next)
	{
	    current = current->next;
	    descend();
	}
	else
	{
	    current = current->parent;
	}
    }
}

void PostOrderLetter::descend()
{
    while (current->children)
    {
	current = current->children;
    }
}

}
