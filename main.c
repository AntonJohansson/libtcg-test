//extern int f(int) ;
//
__attribute__ ((section ("bar")))
int f(int i) {
    char *str = "wow hej 123";
    switch(str[2]) {
    case 'a': return 1;
    case '+': return 2;
    case '1': return 3;
    case '3': return 4;
    case '5': return 5;
    default: return 0;
    }
    if (str[2] < 2) {
        return 1;
    } else {
        return 2;
    }
}

int main(int argc, char **argv) {
    int a = f(0x69);
    return a;
}
