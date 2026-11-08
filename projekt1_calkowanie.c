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

double oblicz_calke_trapezy(double *sygnal, int n, double xmin, double xmax)
{
    double krok = (xmax - xmin) / n;
    double calka = 0.0;

    for (int i = 0; i < n - 1; i++)
    {
        calka += ((sygnal[i] + sygnal[i + 1]) / 2.0) * krok;
    }

    return calka;
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
        printf("\n--- MENU: CALKOWANIE ---\n");
        printf("1. Generowanie sygnalu podstawowego\n");
        printf("2. Wprowadzanie szumu do sygnalu\n");
        printf("3. Zapis do pliku\n");
        printf("4. Filtracja sygnalu\n");
        printf("5. Oblicz calke (metoda trapezow)\n");
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

            if (sygnal_podstawowy != NULL) free(sygnal_podstawowy);
            if (sygnal_zaszumiony != NULL) { free(sygnal_zaszumiony); sygnal_zaszumiony = NULL; }
            if (sygnal_odfiltrowany != NULL) { free(sygnal_odfiltrowany); sygnal_odfiltrowany = NULL; }

            sygnal_podstawowy = malloc(n * sizeof(double));
            if (sygnal_podstawowy != NULL)
            {
                generuj(wsp, xmin, xmax, sygnal_podstawowy, n);
                printf("Wygenerowano sygnal podstawowy (%d probek).\n", n);
            }
            else
            {
                printf("Blad alokacji pamieci!\n");
            }
            break;

        case 2:
            if (sygnal_podstawowy == NULL)
            {
                printf("Blad: Najpierw wygeneruj sygnal podstawowy (opcja 1)!\n");
                break;
            }

            if (sygnal_zaszumiony != NULL) free(sygnal_zaszumiony);
            sygnal_zaszumiony = malloc(n * sizeof(double));

            if (sygnal_zaszumiony != NULL)
            {
                for (int i = 0; i < n; i++) sygnal_zaszumiony[i] = sygnal_podstawowy[i];
                zaszum(sygnal_zaszumiony, n, wsp[0]);
                printf("Wprowadzono szum do sygnalu.\n");
            }
            else
            {
                printf("Blad alokacji pamieci dla sygnalu zaszumionego!\n");
            }
            break;

        case 3:
            if (sygnal_podstawowy == NULL)
            {
                printf("Blad: Brak wygenerowanego sygnalu do zapisu!\n");
                break;
            }

            int wybor_zapisu = 0;
            printf("\nKtory sygnal chcesz zapisac?\n");
            printf("1. Podstawowy\n");
            printf("2. Zaszumiony\n");
            printf("3. Odfiltrowany\n");
            printf("Wybor: ");
            scanf("%d", &wybor_zapisu);

            if (wybor_zapisu == 1) zapisz(sygnal_podstawowy, n, xmin, xmax);
            else if (wybor_zapisu == 2)
            {
                if (sygnal_zaszumiony != NULL) zapisz(sygnal_zaszumiony, n, xmin, xmax);
                else printf("Blad: Sygnal zaszumiony nie zostal jeszcze utworzony (opcja 2)!\n");
            }
            else if (wybor_zapisu == 3)
            {
                if (sygnal_odfiltrowany != NULL) zapisz(sygnal_odfiltrowany, n, xmin, xmax);
                else printf("Blad: Sygnal odfiltrowany nie zostal jeszcze utworzony (opcja 4)!\n");
            }
            else printf("Nieprawidlowy wybor sygnalu.\n");
            break;

        case 4:
            if (sygnal_zaszumiony == NULL)
            {
                printf("Blad: Najpierw utworz sygnal zaszumiony (opcja 2)!\n");
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
            else
            {
                printf("Blad alokacji pamieci dla sygnalu odfiltrowanego!\n");
            }
            break;

        case 5:
            if (sygnal_podstawowy == NULL)
            {
                printf("Blad: Brak danych. Najpierw wygeneruj sygnal podstawowy (opcja 1)!\n");
                break;
            }

            int wybor_sygnalu = 0;
            printf("\nDla ktorego sygnalu obliczyc calke?\n");
            printf("1. Podstawowy\n");
            printf("2. Zaszumiony\n");
            printf("3. Odfiltrowany\n");
            printf("Wybor: ");
            scanf("%d", &wybor_sygnalu);

            if (wybor_sygnalu == 1)
            {
                double calka = oblicz_calke_trapezy(sygnal_podstawowy, n, xmin, xmax);
                printf("Calka metoda trapezow (podstawowy): %f\n", calka);
            }
            else if (wybor_sygnalu == 2)
            {
                if (sygnal_zaszumiony != NULL)
                {
                    double calka = oblicz_calke_trapezy(sygnal_zaszumiony, n, xmin, xmax);
                    printf("Calka metoda trapezow (zaszumiony): %f\n", calka);
                }
                else
                {
                    printf("Blad: Sygnal zaszumiony nie zostal jeszcze utworzony (opcja 2)!\n");
                }
            }
            else if (wybor_sygnalu == 3)
            {
                if (sygnal_odfiltrowany != NULL)
                {
                    double calka = oblicz_calke_trapezy(sygnal_odfiltrowany, n, xmin, xmax);
                    printf("Calka metoda trapezow (odfiltrowany): %f\n", calka);
                }
                else
                {
                    printf("Blad: Sygnal odfiltrowany nie zostal jeszcze utworzony (opcja 4)!\n");
                }
            }
            else
            {
                printf("Nieprawidlowy wybor sygnalu.\n");
            }
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
