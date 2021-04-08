#include <stdio.h>
#include <math.h>
#include <float.h>




int main ()  {

    double x1, y1, r1;
    double x2, y2, r2;
    double distance;
    int check = 0;

    printf("Zadejte parametry kruznice #1:\n");
    if ( scanf ("%lf %lf %lf", &x1, &y1, &r1) != 3
    || r1 <= 0)  {
        printf("Nespravny vstup.\n");
        return 1;
    }



    printf("Zadejte parametry kruznice #2:\n");
    if ( scanf ("%lf %lf %lf", &x2, &y2, &r2) != 3
         || r2 <= 0)  {
        printf("Nespravny vstup.\n");
        return 1;
    }

    distance = sqrt(pow((x2-x1),2)+pow((y2-y1),2));
    if (fabs(distance - (r1 + r2)) < DBL_EPSILON * 1000 * distance) {
        printf("Vnejsi dotyk, zadny prekryv.\n");
        return 0;
    }

    if (distance > (r1 + r2)) {
        printf("Kruznice lezi vne sebe, zadny prekryv.\n");
        return 0;
    }

    if (distance == 0 && r1 == r2) {

        double prekryv = M_PI * r1 * r1;
        printf("Kruznice splyvaji, prekryv: %lf\n", prekryv);
        return 0;
    }

    if(r1 < r2) {
        double tmp = r1;
        r1 = r2;
        r2 = tmp;
        check = 1;
    }

    if (distance + r2 <= r1) {

        if (fabs((distance + r2 - r1)) < 1000 * DBL_EPSILON * r1) {
            double inner_prekryv = M_PI * r2 * r2;
            if (check == 0) { printf("Vnitrni dotyk, kruznice #2 lezi uvnitr kruznice #1, prekryv: %lf\n", inner_prekryv); return 0;}
            printf("Vnitrni dotyk, kruznice #1 lezi uvnitr kruznice #2, prekryv: %lf\n", inner_prekryv); return 0;
        }

        double inner_prekryv1 = M_PI * r2 * r2;
        if (check == 0) { printf("Kruznice #2 lezi uvnitr kruznice #1, prekryv: %lf\n", inner_prekryv1); return 0;}
        printf("Kruznice #1 lezi uvnitr kruznice #2, prekryv: %lf\n", inner_prekryv1); return 0;
    }

    if ((distance < (r1 + r2)) && (distance > (r1 - r2))) {

        double f1 = 2 * acos((r1*r1 - r2*r2 + distance * distance)/(2*r1*distance));
        double f2 = 2 * acos((r2*r2 - r1*r1 + distance * distance)/(2*r2*distance));

        double s1 = (r1*r1*(f1-sin(f1)))/2;
        double s2 = (r2*r2*(f2-sin(f2)))/2;
        double s = s1 + s2;

        printf("Kruznice se protinaji, prekryv: %lf\n", s);
        return 1;



        printf("some shit: %lf %lf %lf %lf %lf\n", f1, f2, s1, s2, s);


    }




/*
    printf ("Bigger area: %lf\n", M_PI * r1 * r1);
    printf ("Smaller area: %lf\n", M_PI * r2 * r2);
    printf("Distance = %lf\n", distance);

    printf ("Prvni kruznice: %lf %lf %lf\n", x1, y1, r1);
    printf ("Druha kruznice: %lf %lf %lf\n", x2, y2, r2);

*/



    return 0;
}
