#include "app.hh"
#include "straight/builder.hh"

int main(int argc, char **argv)
{
    App<straight::Builder> program;
    return program.run("straight", argc, argv);
}
