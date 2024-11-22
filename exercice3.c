#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void split(char *mon_fic, int nb_part)
{
    int fd = open(mon_fic, O_RDONLY);
    if (fd == -1)
    {
        printf("Erreur : Impossible d'ouvrir le fichier source '%s'.\n", mon_fic);
        exit(-1);
    }

    char buffer[1024];
    int cpt = 1;

    while (1) {
        char nomPartie[256];
        snprintf(nomPartie, sizeof(nomPartie), "%s%d", mon_fic, cpt);

        int fic_part = open(nomPartie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fic_part == -1)
        {
            printf("Erreur : Impossible de créer la partie '%s'.\n", nomPartie);
            close(fd);
            exit(-1);
        }

        int r = nb_part;
        while (r > 0)
        {
            int aLire = (r < (int)sizeof(buffer)) ? r : sizeof(buffer);
            int n_lu = read(fd, buffer, aLire);
            if (n_lu == -1)
            {
                printf("Erreur : Lecture échouée sur le fichier source '%s'.\n", mon_fic);
                close(fd);
                close(fic_part);
                exit(-1);
            }
            if (n_lu == 0)
                break;

            int n_ecrit = write(fic_part, buffer, n_lu);
            if (n_ecrit == -1)
            {
                printf("Erreur : Écriture échouée dans la partie '%s'.\n", nomPartie);
                close(fd);
                close(fic_part);
                exit(-1);
            }

            r -= n_ecrit;
        }

        close(fic_part);
        cpt++;
        if (r > 0)
            break;
    }

    close(fd);
    printf("Fichier '%s' découpé en %d parties avec succes!\n", mon_fic, cpt - 1);
}

void join( char *mon_fic, int nbParties, char *parties[])
{
    int fic_join = open(mon_fic, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fic_join == -1)
    {
        printf("Erreur : Impossible de créer le fichier final '%s'.\n", mon_fic);
        exit(-1);
    }

    char buffer[1024];
    for (int i = 0; i < nbParties; i++)
    {
        int fic_part = open(parties[i], O_RDONLY);
        if (fic_part == -1)
        {
            printf("Erreur : Impossible d'ouvrir la partie '%s'.\n", parties[i]);
            close(fic_join);
            exit(-1);
        }

        while (1)
        {
            int n_lu = read(fic_part, buffer, sizeof(buffer));
            if (n_lu == -1)
            {
                printf("Erreur : Lecture échouée sur la partie '%s'.\n", parties[i]);
                close(fic_part);
                close(fic_join);
                exit(-1);
            }
            if (n_lu == 0) break;

            int n_ecrit = write(fic_join, buffer, n_lu);
            if (n_ecrit == -1)
            {
                printf("Erreur : Écriture échouée dans le fichier final '%s'.\n", mon_fic);
                close(fic_part);
                close(fic_join);
                exit(-1);
            }
        }

        close(fic_part);
    }

    close(fic_join);
    printf("Fichier '%s' recréé à partir des parties fournies avec succes!\n", mon_fic);
}

int main(int argc, char *argv[]) {
    if (argc < 2)
    {
        printf("Usage pour découper : %s split mon_fic nb_part\n", argv[0]);
        printf("Usage pour assembler : %s join mon_ficFinal partie1 partie2 ...\n", argv[0]);
        exit(-1);
    }

    if (argc == 3)
    {
         char *mon_fic = argv[1];
         int nb_part = atoi(argv[2]);
        if (nb_part <= 0)
        {
            printf("Erreur : La nb_part doit être un entier positif.\n");
            exit(-1);
        }
        split(mon_fic, nb_part);
    }
    else
    {
         char *mon_fic = argv[1];
         int nbpart = argc-2;
         join(mon_fic,nbpart,&argv[2]);
    }
   return 0;
}
