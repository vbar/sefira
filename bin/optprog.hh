#ifndef optprog_hh
#define optprog_hh

#include "program.hh"

class Answer;

class OptProg : public Program
{
public:
    int run(int argc, char **argv)
    {
        return Program::run("optimistic", argc, argv);
    }

private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2);
};

#endif
