#include "treeiter.hh"

namespace straight
{

TreeIter::TreeIter(xmlNodePtr sentinel, const ITreeAnno &anno):
    Envelope<TreeIterLetter>(new TreeIterLetter(sentinel, anno))
{
}

TreeIter::TreeIter(const ITreeAnno &anno):
    Envelope<TreeIterLetter>(new TreeIterLetter(0, anno))
{
}

TreeIter::TreeIter(const TreeIter &other):
    Envelope<TreeIterLetter>(other)
{
}

TreeIter::TreeIter(TreeIterLetter *letter):
    Envelope<TreeIterLetter>(letter)
{
}

void TreeIter::operator++()
{
    ensure_single();
    get_letter()->inc();
}

void TreeIter::ensure_single()
{
    if (is_shared())
    {
        TreeIter spare(get_letter()->clone());
	swap(spare);
    }
}

}
