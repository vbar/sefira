#ifndef program_hh
#define program_hh

#include <libxml/tree.h>

class Answer;

// common initialization & cleanup for sefira command-line programs
class Program
{
public:
    int run(const char *tailname, int argc, char **argv);

private:
    // returned answer must contain (only) nodes from tree1
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2) = 0;

    void print_lcs(const char *fname1, const char *fname2);
};

#endif
