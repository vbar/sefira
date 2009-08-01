#ifndef strprog_hh
#define strprog_hh

#include "program.hh"

class Answer;

class StrProg : public Program
{
public:
    int run(int argc, char **argv)
    {
        return Program::run("straight", argc, argv);
    }

private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2);
};

#endif
