#ifndef systematic_forrestletter_hh
#define systematic_forrestletter_hh

#include <ostream>
#include <vector>

namespace systematic {

class INode;

class ForrestLetter
{
public:
    typedef std::vector<INode *> TTreeTops;

    TTreeTops treeTops;

    ForrestLetter()
    {
    }

    ForrestLetter(INode *lm, INode *rm);

    bool operator<(const ForrestLetter &other) const;

    bool operator==(const ForrestLetter &other) const;

    INode *pop_right_root();
    INode *pop_right_tree();

    void append_children(INode *node);

    void dump(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const ForrestLetter &f);

}

#endif
