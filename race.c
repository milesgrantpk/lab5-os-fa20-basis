#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ITS 1000
#define FILENAME "out"

FILE *fp;

int ready1 = 0;
int ready2 = 0;

static void *
thread1_fn(void *arg)
{
    int i = 0;
    ready1 = 1;
    do
    {
        usleep(1);
    } while (!ready2);

    while (true)
    {
        fprintf(fp, "Elf\n");
        fprintf(fp, "Magic\n");

        if (i++ > MAX_ITS)
            break;
    }

    return NULL;
}

static void *
thread2_fn(void *arg)
{
    int i = 0;
    ready2 = 1;
    do
    {
        usleep(1);
    } while (!ready1);

    while (true)
    {
        fprintf(fp, "Zombie\n");
        fprintf(fp, "Children\n");

        if (i++ > MAX_ITS)
            break;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int rc;
    pthread_t thread1, thread2;
    fp = fopen(FILENAME, "w");
    if (fp == NULL)
    {
        perror("fopen");
        goto err;
    }

    if (pthread_create(&thread1, NULL, &thread1_fn, NULL) != 0)
    {
        perror("pthread_create thread1");
        goto err_pthread;
    }

    if (pthread_create(&thread2, NULL, &thread2_fn, NULL) != 0)
    {
        perror("pthread_create thread2");
        goto err_pthread;
    }

    if (pthread_join(thread1, NULL) != 0)
    {
        perror("pthread_join thread1");
    }

    if (pthread_join(thread2, NULL) != 0)
    {
        perror("pthread_join thread2");
        goto err_pthread;
    }

    if (fclose(fp) != 0)
    {
        perror("pthread_join thread2");
        goto err;
    };

    /* This error handling pattern helps ensure that pointers are cleaned up 
     * See the Composite Style Guide for more info 
    */
    rc = 0;
done:
    return rc;
err_pthread:
    fclose(fp);
err:
    rc = -1;
    goto done;
}
