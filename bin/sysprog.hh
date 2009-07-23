#ifndef sysprog_hh
#define sysprog_hh

#include "program.hh"

class Answer;

class SysProg : public Program
{
public:
    int run(int argc, char **argv)
    {
      return Program::run("systematic", argc, argv);
    }

private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2);
};

#endif
