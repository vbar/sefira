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
    // Returned answer must be a subset of either tree1, or tree2,
    // depending on the output parameter: tree2 iff second is true,
    // tree1 otherwise.
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2, bool &second) = 0;

    void print_lcs(const char *fname1, const char *fname2);
};

#endif
