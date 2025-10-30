#include <tests.h>
#include <stdio.h>

//RUN: %cheerp_clang -I%S/../unit/ -target cheerp -o %t %s
//RUN: %node %t 2>&1 | %FileCheck %s

int main(){
    char *S = "hello_world";
    assertEqual(S, "hello_world", "Basic test");
    return 0;
}

//CHECK: Basic test : SUCCESS
