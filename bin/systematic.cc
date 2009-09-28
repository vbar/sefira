#include "relapp.hh"
#include "systematic/builder.hh"

int main(int argc, char **argv)
{
    RelApp<systematic::Builder> program;
    return program.run("systematic", argc, argv);
}
