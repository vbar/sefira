#ifndef plastic_hand_hh
#define plastic_hand_hh

#include <libxml/tree.h>
#include "doubleseq.hh"
#include "forrestenum.hh"
#include "forrestscore.hh"
#include "graph.hh"
#include "nodeindex.hh"
#include "relresult.hh"

namespace plastic {

class Decomposition;
class GraphEdge;
class TreeAnno;
class TreeScore;

class Hand
{
private:
    const Decomposition *decomposition;
    const TNodeIndex n;
    const TNodeIndex mt2;
    ForrestEnum forrestEnum;
    DoubleSeq doubleSeq;
    Graph edgeGraph;
    ForrestScore localScore;
    TreeScore *masterScore;

public:
    // all pointer args must not be null and must stay valid for the
    // lifetime of the constructed object
    Hand(xmlNodePtr f, xmlNodePtr g,
	 const Decomposition *decomposition,
	 const TreeAnno &anno2,
	 TreeScore *master_score);

    void compute();

private:
    void cycle_left(TNodeIndex i, xmlNodePtr xi, bool on_main);
    void cycle_right(TNodeIndex i, xmlNodePtr xi, bool on_main);

    RelResult get_score_cond(const GraphEdge *ep) const
    {
	if (!ep)
	{
	  return RelResult();
	}

	return localScore.get(*ep);
    }

    TNodeIndex get_score_sum(const GraphEdge &e) const
    {
        RelResult r = localScore.get(e);
	return r.get_score();
    }

    void update_score(const GraphEdge &e, const RelResult &r);

    // not implemented
    Hand(const Hand &other);
    Hand &operator=(const Hand &other);
};

}

#endif
