int printf(const char* format, ...);
int global_init_var =64;
int global_uninit_var;

void fun1(int i)
{
    printf("%d\n",i);
}

int main(void)
{
    static int static_vat = 85;
    static int static_var2;
    int a = 1;
    int b;
    fun1(static_vat+static_var2+a+b);
    return a;
}
