#include "answer.hh"
#include "program.hh"
#include "optimistic/builder.hh"

class App : public Program
{
public:
    int run(int argc, char **argv)
    {
        return Program::run("optimistic", argc, argv);
    }

private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2);
};

Answer App::get_lcs(xmlNodePtr tree1, xmlNodePtr tree2)
{
    optimistic::Builder builder(tree1, tree2);
    return builder.get_lcs();
}

int main(int argc, char **argv)
{
    App program;
    return program.run(argc, argv);
}
