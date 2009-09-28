#include "relapp.hh"
#include "plastic/builder.hh"

int main(int argc, char **argv)
{
    RelApp<plastic::Builder> program;
    return program.run("plastic", argc, argv);
}
