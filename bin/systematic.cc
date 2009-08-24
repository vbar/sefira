#include "app.hh"
#include "systematic/builder.hh"

int main(int argc, char **argv)
{
    App<systematic::Builder> program;
    return program.run("systematic", argc, argv);
}
