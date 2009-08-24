#include "answer.hh"
#include "program.hh"
#include "straight/builder.hh"

class App : public Program
{
public:
    int run(int argc, char **argv)
    {
        return Program::run("straight", argc, argv);
    }

private:
    virtual Answer get_lcs(xmlNodePtr tree1, xmlNodePtr tree2);
};

Answer App::get_lcs(xmlNodePtr tree1, xmlNodePtr tree2)
{
    straight::Builder builder(tree1, tree2);
    return builder.get_lcs();
}

int main(int argc, char **argv)
{
    App program;
    return program.run(argc, argv);
}
