#ifndef straight_treeiterletter_hh
#define straight_treeiterletter_hh

#include <libxml/tree.h>

namespace straight
{

class ITreeAnno;

class TreeIterLetter
{
private:
    xmlNodePtr sentinel;
    xmlNodePtr current;
    const ITreeAnno &anno;

public:
    TreeIterLetter(xmlNodePtr sentinel, const ITreeAnno &anno);

    TreeIterLetter(xmlNodePtr sentinel, xmlNodePtr current,
		   const ITreeAnno &anno):
        sentinel(sentinel),
	current(current),
	anno(anno)
    {
    }

    TreeIterLetter *clone() const
    {
        return new TreeIterLetter(sentinel, current, anno);
    }

    xmlNodePtr get_current() const { return current; }

    void inc();

private:
    void do_inc();
    void skip_heavy();
};

}

#endif
