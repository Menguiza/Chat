#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

int letras = 32;
int palabras = 7;

char str[64];
char boolean = 1;

struct threadParam_t
{
    mqd_t *queue;
};

void *EnvioMensaje(void *parg)
{
    struct threadParam_t *pargTmp = (struct threadParam_t *)parg;

    while (1)
    {
        if (fgets(str, sizeof(str), stdin))
        {
            if (str[strlen(str) - 1] == '\n')
                str[strlen(str) - 1] = 0;

            if (strncmp(str, "exit", strlen("exit")) == 0)
            {
                boolean = 0;
                break;
            }

            mq_send(*pargTmp->queue, str, strlen(str) + 1, 0);
        }
    }

    mq_close(*pargTmp->queue);
    exit(EXIT_FAILURE);

    return NULL;
}

int main()
{
    pthread_t threadID2;

    mqd_t mq = mq_open("/mq0", O_WRONLY);
    mqd_t mq1 = mq_open("/mq1", O_RDONLY);

    struct threadParam_t threadParam1 = {&mq};

    pthread_create(&threadID2, NULL, &EnvioMensaje, (void *)&threadParam1);

    char buff[32];

    while (boolean == 1)
    {
        mq_receive(mq1, buff, 32, NULL);

        if (buff[strlen(buff) - 1] != '\n')
        {

            if (strncmp(buff, "lista", strlen("lista")) == 0)
            {
                char array1[palabras][letras];
                // Útil para leer el archivo
                char buferArchivo[letras];
                // Abrir el archivo...
                FILE *archivo = fopen("invitados", "r");

                if (archivo == NULL)
                {
                    printf("No se puede abrir el archivo");
                    return 0;
                }

                int contador = 0;

                while (fgets(buferArchivo, letras, archivo))
                {
                    // Remover salto de línea
                    strtok(buferArchivo, "\n");
                    // Copiar la línea a nuestro arreglo, usando el índice
                    memcpy(array1[contador], buferArchivo, letras);
                    contador++;
                }

                fclose(archivo);

                printf("invitados a la farra:\n");

                for (int i = 0; i < palabras; i++)
                {
                    printf("%d.", i + 1);
                    printf("%s\n", array1[i]);
                }
            }
            else if (strncmp(buff, "reversa", strlen("reversa")) == 0)
            {
                char array1[palabras][letras];
                // Útil para leer el archivo
                char buferArchivo[letras];
                // Abrir el archivo...
                FILE *archivo = fopen("invitados", "r");

                if (archivo == NULL)
                {
                    printf("No se puede abrir el archivo");
                    return 0;
                }

                int contador = 0;

                while (fgets(buferArchivo, letras, archivo))
                {
                    // Remover salto de línea
                    strtok(buferArchivo, "\n");
                    // Copiar la línea a nuestro arreglo, usando el índice
                    memcpy(array1[contador], buferArchivo, letras);
                    contador++;
                }

                fclose(archivo);

                printf("invitados a la farra:\n");

                for (int i = palabras - 1; i >= 0; i--)
                {
                    printf("%d.", i + 1);
                    printf("%s\n", array1[i]);
                }
            }
            else
            {
                printf("Message received: %s\n", buff);
            }
        }
    }

    pthread_exit(NULL);

    return (0);
}