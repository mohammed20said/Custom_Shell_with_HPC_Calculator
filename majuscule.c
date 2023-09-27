#include<stdio.h>
#include<ctype.h> /* toupper */
int main()
{
int c;
while((c=getchar())!=EOF)
putchar(toupper(c));

return 1;

}
