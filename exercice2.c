#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

extern int optopt;
extern char* optarg;

int** creation_matrice(int d);
void detruire_matrice(int** mat , int d);
void saisie_matrice(int** mat, int d);
void affichage_matrice(int** mat , int d);
void chargement_matrice_binaire(int fic, int d);
void sauvegarde_matrice_binaire(int fic , int** mat , int d);
void chargement_matrice_texte(int fic ,  int d);
void sauvegarde_matrice_texte(int fic , int** mat , int d);

int** creation_matrice(int d)
{
    int** mat = (int**) malloc(d*sizeof(int*));
    if(mat==NULL)
    {
        printf("Pas d'espace\n");
        exit(-1);
    }
    for(int i=0 ; i<d ; i++)
    {
       mat[i] = (int*) malloc (d*sizeof(int));
        if(mat[i]==NULL)
        {
            printf("Pas d'espace\n");
            exit(-1);
        }
    }
    return mat;

}

void detruire_matrice(int** mat , int d)
{
    if(mat)
    {
        for(int i=0 ; i<d ;i++)
        {
           free(mat[i]);
        }
        free (mat);
    }
    printf("matrice detruite\n");
}

void saisie_matrice(int** mat, int d)
{
    int i,j;
    if(mat)
    {
        srand(time(NULL));
        for(i=0 ; i<d ; i++)
        {
            for(j=0 ; j<d ; j++)
                mat[i][j] = rand()%100;
        }

    }
}

void affichage_matrice(int** mat , int d)
{
    int i,j;
    if(mat)
    {
        for(i=0 ; i<d ; i++)
        {
            for(j=0 ; j<d ; j++)
                printf("%d ", mat[i][j]);
            printf("\n");
        }
    }
}


void chargement_matrice_binaire(int fic, int d)
{
   int n,i,j;
    for(i=0 ; i<d ; i++)
    {
        for(j=0 ; j<d ; j++)
        {
            if(read(fic,&n,sizeof(int)) > 0)
            {
                printf("%d ",n);
            }
        }
         printf("\n");
    }
    printf("chargement_matrice_binaire reussi\n");
}



void sauvegarde_matrice_binaire(int fic, int** mat, int d)
{
    for(int i = 0; i < d; i++)
    {
        if(write(fic, mat[i], d * sizeof(int)) == -1)
        {
            perror("ERREUR D'ECRITURE");
            exit(1);
        }
    }
    printf("sauvegarde_matrice_binaire reussie\n");
}


void chargement_matrice_texte(int fic, int d)
{

    int i, j;
    char character;
    char buff[32];
    for(i=0 ; i<d ; i++)
    {
        for(j=0 ; j<d ; j++)
        {
             while (read(fic, &character, sizeof(char)) > 0)
             {
                if((character!=' ')&&(character!='\n'))
                {
                    sprintf(buff+strlen(buff),"%c",character);
                }
                else
                {
                    int nombre = atoi(buff);
                    printf("%d ",nombre);
                    memset(buff,0,32);
                    break;

                }
             }
        }
        printf("\n");
    }
    printf("chargement_matrice_texte reussi\n");
}

void sauvegarde_matrice_texte(int fic, int** mat, int d)
{
    char tab[40];
    int n;

    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            n = sprintf(tab, "%d ", mat[i][j]);
            if(write(fic, tab, n) == -1)
            {
                perror("Erreur d'ecriture");
                exit(1);
            }
        }
        if(write(fic, "\n", 1) == -1)
        {
            perror("Erreur d'ecriture");
            exit(1);
        }
    }
    printf("sauvegarde_matrice_texte reussie\n");
}


int main(int argc , char* argv[])
{
    if(argc < 2)
    {
        perror("ERREUR: nombre d'arguments insuffisants\n");
        printf("Usage:%s {-a -c -d entier -b -t -f nomfichier}\n",argv[0]);
    }
     int** matrice;
    int opt;
    int fic;
    int dimension;
    char* nomfichier;
    int c_fournie = -1;
    int d_fournie = -1;
    int a_fournie = -1;
    int b_fournie = -1;
    int t_fournie = -1;
    int f_fournie = -1;
    while( (opt=getopt(argc,argv,":acd:btf:") )!=-1)
    {
        switch(opt)
        {
            case'c':c_fournie=1;
                    break;
            case'a':a_fournie=1;
                    break;
            case'd':d_fournie=1;
                    dimension=atoi(optarg);
                    break;
            case'b':b_fournie=1;
                    break;
            case't':t_fournie=1;
                    break;
            case'f':f_fournie=1;
                    nomfichier=strdup(optarg);
                    break;
            case':':printf("Erreur: %c donner un arguments\n",optopt);
                    break;
            case'?':printf("Erreur: option %c inconnue\n",optopt);
                    break;
        }
    }


    if( (c_fournie==1)&&(d_fournie==1) )
    {

        matrice=creation_matrice(dimension);
        saisie_matrice(matrice,dimension);
        affichage_matrice(matrice,dimension);
    }
   if((f_fournie==1)&&(c_fournie==1))
    {
            if((t_fournie==1)&&(b_fournie==1))
            {
                printf("Erreur deux modes simultanement impossible\n");
                exit(-1);
            }
            else
            {
                fic = open(nomfichier,O_RDWR|O_CREAT|O_TRUNC,0644);
                if(fic == -1)
                {
                    perror("Erreur lors de l'ouverture du fichier\n");
                    exit(-1);
                }
                if(t_fournie==1)
                {
                    sauvegarde_matrice_texte(fic,matrice,dimension);
                }
                if(b_fournie==1)
                {
                    sauvegarde_matrice_binaire(fic,matrice,dimension);
                }
                if((t_fournie==-1)&&(b_fournie==-1))
                {
                    sauvegarde_matrice_binaire(fic,matrice,dimension); //sauvegarde en mode binaire par defaut
                }
            }
            close(fic);
    }
    if(a_fournie==1)
    {

         fic = open(nomfichier,O_RDONLY|O_CREAT,0644);
         if(fic == -1)
         {
            perror("Erreur lors de l'ouverture du fichier\n");
            exit(-1);
         }

         if(t_fournie==1)
         {
            chargement_matrice_texte(fic,dimension);
         }
         if(b_fournie==1)
         {
            chargement_matrice_binaire(fic,dimension);
         }
         if((t_fournie==-1)&&(b_fournie==-1))//chargement en mode binaire par defaut
         {
            chargement_matrice_binaire(fic,dimension);
         }
         close(fic);
    }
   detruire_matrice(matrice,dimension);
    return 0;

}
