#include <stdio.h>

int main(int argc, char const *argv[])
{
    nullptr_t a = nullptr;
    typeof(a) b = a;
    printf("Hello, world! %d\n", a);

    return 0;
}
