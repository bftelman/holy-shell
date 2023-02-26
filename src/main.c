#include "hshell.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    srand(time(0));
    hshell_loop();
    return 0;
}
