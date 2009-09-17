#ifndef plastic_hand_hh
#define plastic_hand_hh

#include <libxml/tree.h>
#include "answer.hh"
#include "doubleseq.hh"
#include "forrestenum.hh"
#include "forrestscore.hh"
#include "graph.hh"
#include "nodeindex.hh"

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

    Answer get_score(const GraphEdge &e) const;

    Answer get_score_cond(const GraphEdge *ep) const
    {
	if (!ep)
	{
	  return Answer();
	}

	return get_score(*ep);
    }

    TNodeIndex get_score_sum(const GraphEdge &e) const
    {
        Answer a = get_score(e);
	return a.get_score();
    }

    void insert_score(const GraphEdge &e, const Answer &a);

    // not implemented
    Hand(const Hand &other);
    Hand &operator=(const Hand &other);
};

}

#endif
