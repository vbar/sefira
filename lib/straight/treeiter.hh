#ifndef straight_treeiter_hh
#define straight_treeiter_hh

#include <libxml/tree.h>
#include "envelope.hh"
#include "treeiterletter.hh"

namespace straight
{

class ITreeAnno;

// iterates over TopLight(sentinel)
class TreeIter : private Envelope<TreeIterLetter>
{
public:
    TreeIter(xmlNodePtr sentinel, const ITreeAnno &anno);
    TreeIter(const ITreeAnno &anno);
    TreeIter(const TreeIter &other);

private:
    TreeIter(TreeIterLetter *letter);

public:
    xmlNodePtr operator*() const
    {
        return get_letter()->get_current();
    }  

    bool operator==(const TreeIter &other) const
    {
        return get_letter()->get_current() ==
	    other.get_letter()->get_current();
    }

    bool operator!=(const TreeIter &other) const
    {
        return !(*this == other);
    }

    void operator++();

private:
    void ensure_single();
};

}

#endif
