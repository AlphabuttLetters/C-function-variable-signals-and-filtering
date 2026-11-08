

/* ==========================================================================
   PROGRAM 1: DECYMACJA SYGNALU
   (Usuwanie probek bedacych wielokrotnoscia zadanego kroku k, np. 4)
   ========================================================================== */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <time.h>

void pobierz_wspolczynniki_p1(double *wsp)
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

void pobierz_granice_p1(double *xmin, double *xmax)
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

void pobierz_rozmiar_p1(int *n)
{
    printf("Podaj liczbe probek: ");
    scanf("%d", n);

    while (*n < 10)
    {
        printf("Za malo probek (min. 10). Podaj ponownie: ");
        scanf("%d", n);
    }
}

void generuj_p1(double *wsp, double xmin, double xmax, double *sygnal, int n)
{
    double krok = (xmax - xmin) / n;
    for (int i = 0; i < n; i++)
    {
        double x = xmin + i * krok;
        sygnal[i] = wsp[0] * sin(wsp[1] * x * cos(wsp[2] * x + wsp[3]) + wsp[4]) + wsp[5];
    }
}

void zaszum_p1(double *sygnal, int n, double A)
{
    /* Maksymalna amplituda na podstawie wspolczynnika A (0.5 * |A|), ale nie wiecej niz 5 */
    double max_szum = 0.5 * fabs(A);
    if (max_szum > 5.0)
    {
        max_szum = 5.0;
    }

    for (int i = 0; i < n; i++)
    {
        double amplituda = ((double)rand() / RAND_MAX) * max_szum;
        /* Losowanie dla kazdej probki: w gore (+1) czy w dol (-1) */
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

void filtr_medianowy_p1(double *sygnal, int n)
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

void filtr_sredniej_ruchomej_p1(double *sygnal, int n)
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

void decymuj_p1(double **sygnal, int *n, int k)
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
        printf("Blad alokacji pamieci!\n");
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

void zapisz_p1(double *sygnal, int n, double xmin, double xmax)
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

int main_program_1()
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
        printf("1. Generowanie sygnalu\n");
        printf("2. Wprowadzanie szumu\n");
        printf("3. Filtracja sygnalu\n");
        printf("4. Decymacja sygnalu (usun wielokrotnosci k)\n");
        printf("5. Zapis do pliku\n");
        printf("0. Zakonczenie programu\n");
        printf("Wybor: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 0:
            printf("Koniec programu.\n");
            break;
        case 1:
            pobierz_wspolczynniki_p1(wsp);
            pobierz_granice_p1(&xmin, &xmax);
            pobierz_rozmiar_p1(&n);

            if (sygnal != NULL) free(sygnal);
            sygnal = malloc(n * sizeof(double));

            if (sygnal != NULL)
            {
                generuj_p1(wsp, xmin, xmax, sygnal, n);
                printf("Wygenerowano sygnal (%d probek).\n", n);
            }
            break;
        case 2:
            if (sygnal != NULL)
            {
                zaszum_p1(sygnal, n, wsp[0]);
                printf("Wprowadzono szum (max 5, losowanie gora/dol dla kazdej probki).\n");
            }
            else printf("Blad: Najpierw wygeneruj sygnal!\n");
            break;
        case 3:
            if (sygnal != NULL)
            {
                filtr_medianowy_p1(sygnal, n);
                filtr_sredniej_ruchomej_p1(sygnal, n);
                printf("Odfiltrowano sygnal.\n");
            }
            else printf("Blad: Najpierw wygeneruj sygnal!\n");
            break;
        case 4:
            if (sygnal != NULL)
            {
                int k;
                printf("Podaj krok decymacji k (np. 4): ");
                scanf("%d", &k);
                decymuj_p1(&sygnal, &n, k);
            }
            else printf("Blad: Najpierw wygeneruj sygnal!\n");
            break;
        case 5:
            if (sygnal != NULL) zapisz_p1(sygnal, n, xmin, xmax);
            else printf("Blad: Brak sygnalu do zapisu!\n");
            break;
        default:
            printf("Nieprawidlowa opcja!\n");
        }
    }

    if (sygnal != NULL) free(sygnal);
    return 0;
}


/* ==========================================================================
   PROGRAM 2: CALKOWANIE NUMERYCZNE METODA TRAPEZOW
   (Wybór jednego z 3 sygnałów: podstawowy, zaszumiony, odfiltrowany)
   ========================================================================== */

void pobierz_wspolczynniki_p2(double *wsp)
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

void pobierz_granice_p2(double *xmin, double *xmax)
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

void pobierz_rozmiar_p2(int *n)
{
    printf("Podaj liczbe probek: ");
    scanf("%d", n);

    while (*n < 10)
    {
        printf("Za malo probek (min. 10). Podaj ponownie: ");
        scanf("%d", n);
    }
}

void generuj_p2(double *wsp, double xmin, double xmax, double *sygnal, int n)
{
    double krok = (xmax - xmin) / n;
    for (int i = 0; i < n; i++)
    {
        double x = xmin + i * krok;
        sygnal[i] = wsp[0] * sin(wsp[1] * x * cos(wsp[2] * x + wsp[3]) + wsp[4]) + wsp[5];
    }
}

void zaszum_p2(double *sygnal, int n, double A)
{
    /* Maksymalna amplituda na podstawie wspolczynnika A (0.5 * |A|), ale nie wiecej niz 5 */
    double max_szum = 0.5 * fabs(A);
    if (max_szum > 5.0)
    {
        max_szum = 5.0;
    }

    for (int i = 0; i < n; i++)
    {
        double amplituda = ((double)rand() / RAND_MAX) * max_szum;
        /* Losowanie dla kazdej probki: w gore (+1) czy w dol (-1) */
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

void filtr_medianowy_p2(double *sygnal, int n)
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

void filtr_sredniej_ruchomej_p2(double *sygnal, int n)
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

double oblicz_calke_trapezy_p2(double *sygnal, int n, double xmin, double xmax)
{
    double krok = (xmax - xmin) / n;
    double calka = 0.0;

    for (int i = 0; i < n - 1; i++)
    {
        calka += ((sygnal[i] + sygnal[i + 1]) / 2.0) * krok;
    }

    return calka;
}

void zapisz_p2(double *sygnal, int n, double xmin, double xmax)
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

int main_program_2()
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
        printf("2. Wprowadzanie szumu\n");
        printf("3. Filtracja sygnalu\n");
        printf("4. Oblicz calke metoda trapezow\n");
        printf("5. Zapis do pliku\n");
        printf("0. Zakonczenie programu\n");
        printf("Wybor: ");
        scanf("%d", &menu);

        switch (menu)
        {
        case 0:
            printf("Koniec programu.\n");
            break;
        case 1:
            pobierz_wspolczynniki_p2(wsp);
            pobierz_granice_p2(&xmin, &xmax);
            pobierz_rozmiar_p2(&n);

            if (sygnal_podstawowy != NULL) free(sygnal_podstawowy);
            if (sygnal_zaszumiony != NULL) { free(sygnal_zaszumiony); sygnal_zaszumiony = NULL; }
            if (sygnal_odfiltrowany != NULL) { free(sygnal_odfiltrowany); sygnal_odfiltrowany = NULL; }

            sygnal_podstawowy = malloc(n * sizeof(double));
            if (sygnal_podstawowy != NULL)
            {
                generuj_p2(wsp, xmin, xmax, sygnal_podstawowy, n);
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
                zaszum_p2(sygnal_zaszumiony, n, wsp[0]);
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
                filtr_medianowy_p2(sygnal_odfiltrowany, n);
                filtr_sredniej_ruchomej_p2(sygnal_odfiltrowany, n);
                printf("Odfiltrowano sygnal.\n");
            }
            break;
        case 4:
            if (sygnal_podstawowy == NULL)
            {
                printf("Blad: Brak danych. Najpierw wygeneruj sygnal podstawowy!\n");
                break;
            }
            int wybor_sygnalu;
            printf("\nDla ktorego sygnalu obliczyc calke?\n");
            printf("1. Podstawowy\n");
            printf("2. Zaszumiony\n");
            printf("3. Odfiltrowany\n");
            printf("Wybor: ");
            scanf("%d", &wybor_sygnalu);

            if (wybor_sygnalu == 1)
            {
                double calka = oblicz_calke_trapezy_p2(sygnal_podstawowy, n, xmin, xmax);
                printf("Calka metoda trapezow (podstawowy): %f\n", calka);
            }
            else if (wybor_sygnalu == 2)
            {
                if (sygnal_zaszumiony != NULL)
                {
                    double calka = oblicz_calke_trapezy_p2(sygnal_zaszumiony, n, xmin, xmax);
                    printf("Calka metoda trapezow (zaszumiony): %f\n", calka);
                }
                else printf("Blad: Sygnal zaszumiony nie zostal jeszcze utworzony!\n");
            }
            else if (wybor_sygnalu == 3)
            {
                if (sygnal_odfiltrowany != NULL)
                {
                    double calka = oblicz_calke_trapezy_p2(sygnal_odfiltrowany, n, xmin, xmax);
                    printf("Calka metoda trapezow (odfiltrowany): %f\n", calka);
                }
                else printf("Blad: Sygnal odfiltrowany nie zostal jeszcze utworzony!\n");
            }
            else printf("Nieprawidlowy wybor sygnalu.\n");
            break;
        case 5:
            if (sygnal_podstawowy != NULL) zapisz_p2(sygnal_podstawowy, n, xmin, xmax);
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


/* ==========================================================================
   PROGRAM 3: ROZNICA SYGNALOW I ZAPIS DO EXCELA (.CSV)
   (Wyznacza roznice y_oryginal - y_odfiltrowany dla kazdej probki i zapisuje)
   ========================================================================== */

void pobierz_wspolczynniki_p3(double *wsp)
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

void pobierz_granice_p3(double *xmin, double *xmax)
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

void pobierz_rozmiar_p3(int *n)
{
    printf("Podaj liczbe probek: ");
    scanf("%d", n);

    while (*n < 10)
    {
        printf("Za malo probek (min. 10). Podaj ponownie: ");
        scanf("%d", n);
    }
}

void generuj_p3(double *wsp, double xmin, double xmax, double *sygnal, int n)
{
    double krok = (xmax - xmin) / n;
    for (int i = 0; i < n; i++)
    {
        double x = xmin + i * krok;
        sygnal[i] = wsp[0] * sin(wsp[1] * x * cos(wsp[2] * x + wsp[3]) + wsp[4]) + wsp[5];
    }
}

void zaszum_p3(double *sygnal, int n, double A)
{
    /* Maksymalna amplituda na podstawie wspolczynnika A (0.5 * |A|), ale nie wiecej niz 5 */
    double max_szum = 0.5 * fabs(A);
    if (max_szum > 5.0)
    {
        max_szum = 5.0;
    }

    for (int i = 0; i < n; i++)
    {
        double amplituda = ((double)rand() / RAND_MAX) * max_szum;
        /* Losowanie dla kazdej probki: w gore (+1) czy w dol (-1) */
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

void filtr_medianowy_p3(double *sygnal, int n)
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

void filtr_sredniej_ruchomej_p3(double *sygnal, int n)
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

void oblicz_i_zapisz_roznice_p3(double *podstawowy, double *odfiltrowany, int n, double xmin, double xmax)
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

void zapisz_p3(double *sygnal, int n, double xmin, double xmax)
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

int main_program_3()
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
            pobierz_wspolczynniki_p3(wsp);
            pobierz_granice_p3(&xmin, &xmax);
            pobierz_rozmiar_p3(&n);

            if (sygnal_podstawowy != NULL) free(sygnal_podstawowy);
            if (sygnal_zaszumiony != NULL) { free(sygnal_zaszumiony); sygnal_zaszumiony = NULL; }
            if (sygnal_odfiltrowany != NULL) { free(sygnal_odfiltrowany); sygnal_odfiltrowany = NULL; }

            sygnal_podstawowy = malloc(n * sizeof(double));
            if (sygnal_podstawowy != NULL)
            {
                generuj_p3(wsp, xmin, xmax, sygnal_podstawowy, n);
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
                zaszum_p3(sygnal_zaszumiony, n, wsp[0]);
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
                filtr_medianowy_p3(sygnal_odfiltrowany, n);
                filtr_sredniej_ruchomej_p3(sygnal_odfiltrowany, n);
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
            oblicz_i_zapisz_roznice_p3(sygnal_podstawowy, sygnal_odfiltrowany, n, xmin, xmax);
            break;
        case 5:
            if (sygnal_podstawowy != NULL) zapisz_p3(sygnal_podstawowy, n, xmin, xmax);
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

/* ==========================================================================
   GLOWNA FUNKCJA MAIN SELEKTORA (OPCJONALNA DO SZYBKIEGO TESTOWANIA)
   Pozwala uruchomic dowolny z 3 powyzszych programow.
   Jezeli kompilujesz dany program pojedynczo w Visual Studio / Code::Blocks,
   zmien nazwe wybranej funkcji (np. main_program_1) na 'main', a ten blok usun.
   ========================================================================== */
int main()
{
    int prog = 0;
    printf("===========================================\n");
    printf("Wybierz program do uruchomienia:\n");
    printf("1. Decymacja sygnalu\n");
    printf("2. Calkowanie metoda trapezow (3 sygnaly)\n");
    printf("3. Obliczanie roznicy i zapis do Excela\n");
    printf("Wybor: ");
    if (scanf("%d", &prog) == 1)
    {
        if (prog == 1) return main_program_1();
        if (prog == 2) return main_program_2();
        if (prog == 3) return main_program_3();
    }
    return 0;
}
