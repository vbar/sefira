#include "app.hh"
#include "optimistic/builder.hh"

int main(int argc, char **argv)
{
    App<optimistic::Builder> program;
    return program.run("optimistic", argc, argv);
}
