#include <stdio.h> //libraries
#include <cs50.h>

int main(void)
{
    // asking for the name of user
    string name = get_string("What's your name ? : ");
    // print user's name
    printf("hello, %s\n", name);
}
