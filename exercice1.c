#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int f(int n)
{
    int p = 1;
    for(int i=0 ; i<n ; i++)
         p = p*2;
    return p;
}

int g(int n)
{
    return 2*n;
}

int main(int argc , char* argv[])
{
    extern char* optarg;
    extern int optopt;
    int opt;
    int arg_f = -1 , arg_g = -1 ; //recuperation des arguments de f(n) et g(n)
    int func_1 = -1; // booleen qui indique quelle fonction executee en premier
    int exist_g = -1, exist_f = -1; // gere si les fonctions f et g sont founies



    if((argc >= 3)&&(argc <= 4))
    {
        while((opt=getopt(argc , argv ,":f:g:"))!=-1)
        {
            switch(opt)
            {
                case 'f':
                    arg_f = atoi(optarg);
                    func_1 = 1;
                    exist_f = 1;
                    break;
                case 'g':
                    arg_g = atoi(optarg);
                    func_1 = 0;
                    exist_g = 1;
                    break;
                case ':':
                        if(exist_f && optopt!= 'f')
                            exist_g = 1;
                        if(exist_g && optopt!= 'g')
                            exist_f = 1;
                        break;
                case '?':
                        printf("Erreur oprtion %c inconnu \n" , optopt);
                        break;

            }
        }


           if(argc == 3)
            {
                if((arg_f != -1) && (arg_g == -1) && (exist_f == 1))
                    printf(" f(n) =  2^n -> f(%d) = %d",arg_f,f(arg_f));
                if((arg_f == -1) && (arg_g != -1) && (exist_g == 1))
                    printf(" g(n) = 2*n -> g(%d) = %d",arg_g,g(arg_g));
            }
            else
            {
                if ( (exist_f == 1) && (exist_g == 1))
                {
                    if((func_1 == 1) && (arg_f != -1))
                        printf(" f(n) =  2^n et g(n) = 2*n -> f(g(%d)) = %d",arg_f,f(g(arg_f)));
                    if((func_1 == 0) &&(arg_g != -1) )
                        printf(" f(n) =  2^n et g(n) = 2*n -> g(f(%d)) = %d",arg_g,g(f(arg_g)));
                }
                else
                {
                    printf("la meme option est fournie deux fois \n");
                }

            }
    }
    else
    {
        perror ("Nombre d'arguments insuffisant\n");
                 printf("Usage:\n %s -f entier :f(n) \n %s -g entier : g(n) \n %s -f entier -g : f(g(n)) \n %s -g entier -f : g(f(n)) \n",argv[0],argv[0],argv[0],argv[0]);

    }

    return 0;

}
