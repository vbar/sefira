#include "postorder.hh"

namespace plastic
{

PostOrder::PostOrder():
    Envelope<PostOrderLetter>(new PostOrderLetter())
{
}

PostOrder::PostOrder(xmlNodePtr sentinel):
    Envelope<PostOrderLetter>(new PostOrderLetter(sentinel))
{
}

PostOrder::PostOrder(const PostOrder &other):
    Envelope<PostOrderLetter>(other)
{
}

PostOrder::PostOrder(PostOrderLetter *letter):
    Envelope<PostOrderLetter>(letter)
{
}

void PostOrder::operator++()
{
    ensure_single();
    get_letter()->inc();
}

void PostOrder::ensure_single()
{
    if (is_shared())
    {
        PostOrder spare(get_letter()->clone());
	swap(spare);
    }
}

}
