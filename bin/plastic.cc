#include "app.hh"
#include "plastic/builder.hh"

int main(int argc, char **argv)
{
    App<plastic::Builder> program;
    return program.run("plastic", argc, argv);
}
