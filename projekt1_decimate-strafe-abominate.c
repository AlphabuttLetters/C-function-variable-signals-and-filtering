#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <time.h>

void pobierz_wspolczynniki(double *wsp)
{
    printf("Podaj A: ");
    scanf("%lf", &wsp[0]);
    while (wsp[0] == 0)
    {
        printf("Blad, A nie moze byc 0. Podaj A: ");
        scanf("%lf", &wsp[0]);
    }

    printf("Podaj B: ");
    scanf("%lf", &wsp[1]);
    while (wsp[1] == 0)
    {
        printf("Blad, B nie moze byc 0. Podaj B: ");
        scanf("%lf", &wsp[1]);
    }

    printf("Podaj C: ");
    scanf("%lf", &wsp[2]);
    printf("Podaj D: ");
    scanf("%lf", &wsp[3]);
    printf("Podaj E: ");
    scanf("%lf", &wsp[4]);
    printf("Podaj F: ");
    scanf("%lf", &wsp[5]);
}

void pobierz_granice(double *xmin, double *xmax)
{
    printf("Podaj xmin: ");
    scanf("%lf", xmin);
    printf("Podaj xmax: ");
    scanf("%lf", xmax);

    while (*xmin >= *xmax)
    {
        printf("Blad, xmin musi byc mniejsze od xmax.\n");
        printf("Podaj xmin: ");
        scanf("%lf", xmin);
        printf("Podaj xmax: ");
        scanf("%lf", xmax);
    }
}

void pobierz_rozmiar(int *n)
{
    printf("Podaj liczbe probek: ");
    scanf("%d", n);

    while (*n < 10)
    {
        printf("Za malo probek (min. 10). Podaj ponownie: ");
        scanf("%d", n);
    }
}

void generuj(double *wsp, double xmin, double xmax, double *sygnal, int n)
{
    double krok = (xmax - xmin) / n;
    for (int i = 0; i < n; i++)
    {
        double x = xmin + i * krok;
        sygnal[i] = wsp[0] * sin(wsp[1] * x * cos(wsp[2] * x + wsp[3]) + wsp[4]) + wsp[5];
    }
}

void zaszum(double *sygnal, int n, double A)
{
    double max_szum = 0.5 * fabs(A);
    if (max_szum > 5.0)
    {
        max_szum = 5.0;
    }

    for (int i = 0; i < n; i++)
    {
        double amplituda = ((double)rand() / RAND_MAX) * max_szum;
        if (rand() % 2 == 0)
        {
            sygnal[i] += amplituda;
        }
        else
        {
            sygnal[i] -= amplituda;
        }
    }
}

void filtr_medianowy(double *sygnal, int n)
{
    double *kopia = malloc(n * sizeof(double));
    if (kopia == NULL) return;

    for (int i = 0; i < n; i++) kopia[i] = sygnal[i];

    for (int i = 2; i < n - 2; i++)
    {
        double okno[5] = { kopia[i - 2], kopia[i - 1], kopia[i], kopia[i + 1], kopia[i + 2] };
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4 - j; k++)
            {
                if (okno[k] > okno[k + 1])
                {
                    double temp = okno[k];
                    okno[k] = okno[k + 1];
                    okno[k + 1] = temp;
                }
            }
        }
        sygnal[i] = okno[2];
    }
    free(kopia);
}

void filtr_sredniej_ruchomej(double *sygnal, int n)
{
    double *kopia = malloc(n * sizeof(double));
    if (kopia == NULL) return;

    for (int i = 0; i < n; i++) kopia[i] = sygnal[i];

    for (int i = 2; i < n - 2; i++)
    {
        sygnal[i] = (kopia[i - 2] + kopia[i - 1] + kopia[i] + kopia[i + 1] + kopia[i + 2]) / 5.0;
    }
    free(kopia);
}

void decymuj(double **sygnal, int *n, int k)
{
    if (k <= 1 || *n < k)
    {
        printf("Blad: Krok decymacji musi byc wiekszy od 1 i mniejszy od liczby probek.\n");
        return;
    }

    int nowy_n = 0;
    for (int i = 0; i < *n; i++)
    {
        if ((i + 1) % k != 0)
        {
            nowy_n++;
        }
    }

    double *nowy_sygnal = malloc(nowy_n * sizeof(double));
    if (nowy_sygnal == NULL)
    {
        printf("Blad alokacji pamieci podczas decymacji!\n");
        return;
    }

    int idx = 0;
    for (int i = 0; i < *n; i++)
    {
        if ((i + 1) % k != 0)
        {
            nowy_sygnal[idx++] = (*sygnal)[i];
        }
    }

    free(*sygnal);
    *sygnal = nowy_sygnal;
    *n = nowy_n;

    printf("Zdecymowano sygnal. Nowa liczba probek: %d\n", *n);
}

void zapisz(double *sygnal, int n, double xmin, double xmax)
{
    FILE *plik = fopen("sygnal.csv", "w");
    if (plik != NULL)
    {
        double krok = (xmax - xmin) / n;
        for (int i = 0; i < n; i++)
        {
            double x = xmin + i * krok;
            fprintf(plik, "%d;%f;%f\n", i, x, sygnal[i]);
        }
        fclose(plik);
        printf("Zapisano sygnal do pliku sygnal.csv\n");
    }
    else
    {
        printf("Blad otwarcia pliku.\n");
    }
}

int main()
{
    setlocale(LC_ALL, "polish_poland");
    srand((unsigned int)time(NULL));

    double wsp[6];
    double xmin = 0.0, xmax = 0.0;
    int n = 0;
    double *sygnal = NULL;

    int menu = -1;
    while (menu != 0)
    {
        printf("\n--- MENU: DECYMACJA ---\n");
        printf("1. Generowanie sygnalu podstawowego\n");
        printf("2. Wprowadzanie szumu do sygnalu\n");
        printf("3. Filtracja sygnalu\n");
        printf("4. Decymacja sygnalu podstawowego\n");
        printf("5. Zapis do pliku\n");
        printf("0. Zakonczenie programu\n");
        printf("Twoj wybor: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 0:
            printf("Zakonczenie programu\n");
            break;
        case 1:
            pobierz_wspolczynniki(wsp);
            pobierz_granice(&xmin, &xmax);
            pobierz_rozmiar(&n);

            if (sygnal != NULL) free(sygnal);
            sygnal = malloc(n * sizeof(double));

            if (sygnal != NULL)
            {
                generuj(wsp, xmin, xmax, sygnal, n);
                printf("Wygenerowano sygnal (%d probek).\n", n);
            }
            else
            {
                printf("Blad alokacji pamieci!\n");
            }
            break;
        case 2:
            if (sygnal != NULL)
            {
                zaszum(sygnal, n, wsp[0]);
                printf("Wprowadzono szum do sygnalu.\n");
            }
            else
            {
                printf("Blad: Najpierw wygeneruj sygnal podstawowy (opcja 1)!\n");
            }
            break;
        case 3:
            if (sygnal != NULL)
            {
                filtr_medianowy(sygnal, n);
                filtr_sredniej_ruchomej(sygnal, n);
                printf("Odfiltrowano sygnal.\n");
            }
            else
            {
                printf("Blad: Najpierw wygeneruj sygnal (opcja 1)!\n");
            }
            break;
        case 4:
            if (sygnal != NULL)
            {
                int k;
                printf("Podaj krok decymacji k (np. 4): ");
                scanf("%d", &k);
                decymuj(&sygnal, &n, k);
            }
            else
            {
                printf("Blad: Najpierw wygeneruj sygnal (opcja 1)!\n");
            }
            break;
        case 5:
            if (sygnal != NULL)
            {
                zapisz(sygnal, n, xmin, xmax);
            }
            else
            {
                printf("Blad: Brak sygnalu do zapisu! Najpierw wygeneruj sygnal (opcja 1)!\n");
            }
            break;
        default:
            printf("Nieprawidlowa opcja!\n");
        }
    }

    if (sygnal != NULL) free(sygnal);
    return 0;
}
