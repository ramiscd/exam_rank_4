
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	
}

#include <stdio.h>
int main(void)
{
    int   fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    char  buf[100];
    int   n = read(fd, buf, 99);

    if (n > 0)
    {
        buf[n] = '\0';
        printf("%s", buf);
    }
    close(fd);
}
