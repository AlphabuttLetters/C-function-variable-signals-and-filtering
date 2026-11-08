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

void oblicz_i_zapisz_roznice(double *podstawowy, double *odfiltrowany, int n, double xmin, double xmax)
{
    double *roznica = malloc(n * sizeof(double));
    if (roznica == NULL)
    {
        printf("Blad alokacji pamieci dla tablicy roznicy!\n");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        roznica[i] = podstawowy[i] - odfiltrowany[i];
    }

    FILE *plik = fopen("roznica_excel.csv", "w");
    if (plik != NULL)
    {
        fprintf(plik, "Indeks;X;Podstawowy;Odfiltrowany;Roznica\n");
        double krok = (xmax - xmin) / n;
        for (int i = 0; i < n; i++)
        {
            double x = xmin + i * krok;
            fprintf(plik, "%d;%f;%f;%f;%f\n", i, x, podstawowy[i], odfiltrowany[i], roznica[i]);
        }
        fclose(plik);
        printf("Zapisano dane do pliku roznica_excel.csv\n");
    }
    else
    {
        printf("Blad otwarcia pliku do zapisu!\n");
    }

    free(roznica);
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

    double *sygnal_podstawowy = NULL;
    double *sygnal_zaszumiony = NULL;
    double *sygnal_odfiltrowany = NULL;

    int menu = -1;
    while (menu != 0)
    {
        printf("\n--- MENU: ROZNICA SYGNALOW ---\n");
        printf("1. Generowanie sygnalu podstawowego\n");
        printf("2. Wprowadzanie szumu\n");
        printf("3. Filtracja sygnalu\n");
        printf("4. Oblicz roznice (oryginal - odfiltrowany) i zapisz do Excela\n");
        printf("5. Zapis sygnalu podstawowego do pliku\n");
        printf("0. Zakonczenie programu\n");
        printf("Wybor: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 0:
            printf("Koniec programu.\n");
            break;
        case 1:
            pobierz_wspolczynniki(wsp);
            pobierz_granice(&xmin, &xmax);
            pobierz_rozmiar(&n);

            if (sygnal_podstawowy != NULL) free(sygnal_podstawowy);
            if (sygnal_zaszumiony != NULL) { free(sygnal_zaszumiony); sygnal_zaszumiony = NULL; }
            if (sygnal_odfiltrowany != NULL) { free(sygnal_odfiltrowany); sygnal_odfiltrowany = NULL; }

            sygnal_podstawowy = malloc(n * sizeof(double));
            if (sygnal_podstawowy != NULL)
            {
                generuj(wsp, xmin, xmax, sygnal_podstawowy, n);
                printf("Wygenerowano sygnal podstawowy (%d probek).\n", n);
            }
            break;
        case 2:
            if (sygnal_podstawowy == NULL)
            {
                printf("Blad: Najpierw wygeneruj sygnal podstawowy!\n");
                break;
            }
            if (sygnal_zaszumiony != NULL) free(sygnal_zaszumiony);
            sygnal_zaszumiony = malloc(n * sizeof(double));
            if (sygnal_zaszumiony != NULL)
            {
                for (int i = 0; i < n; i++) sygnal_zaszumiony[i] = sygnal_podstawowy[i];
                zaszum(sygnal_zaszumiony, n, wsp[0]);
                printf("Wprowadzono szum (max 5, losowanie gora/dol dla kazdej probki).\n");
            }
            break;
        case 3:
            if (sygnal_zaszumiony == NULL)
            {
                printf("Blad: Najpierw wygeneruj sygnal zaszumiony!\n");
                break;
            }
            if (sygnal_odfiltrowany != NULL) free(sygnal_odfiltrowany);
            sygnal_odfiltrowany = malloc(n * sizeof(double));
            if (sygnal_odfiltrowany != NULL)
            {
                for (int i = 0; i < n; i++) sygnal_odfiltrowany[i] = sygnal_zaszumiony[i];
                filtr_medianowy(sygnal_odfiltrowany, n);
                filtr_sredniej_ruchomej(sygnal_odfiltrowany, n);
                printf("Odfiltrowano sygnal.\n");
            }
            break;
        case 4:
            if (sygnal_podstawowy == NULL)
            {
                printf("Blad: Brak sygnalu podstawowego!\n");
                break;
            }
            if (sygnal_odfiltrowany == NULL)
            {
                printf("Blad: Brak sygnalu odfiltrowanego (najpierw wybierz opcje 2 i 3)!\n");
                break;
            }
            oblicz_i_zapisz_roznice(sygnal_podstawowy, sygnal_odfiltrowany, n, xmin, xmax);
            break;
        case 5:
            if (sygnal_podstawowy != NULL) zapisz(sygnal_podstawowy, n, xmin, xmax);
            else printf("Blad: Brak sygnalu do zapisu!\n");
            break;
        default:
            printf("Nieprawidlowa opcja!\n");
        }
    }

    if (sygnal_podstawowy != NULL) free(sygnal_podstawowy);
    if (sygnal_zaszumiony != NULL) free(sygnal_zaszumiony);
    if (sygnal_odfiltrowany != NULL) free(sygnal_odfiltrowany);

    return 0;
}
