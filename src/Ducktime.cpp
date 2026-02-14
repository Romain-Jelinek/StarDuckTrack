#include "Ducktime.h"
#include <ctime>
#include <iostream>
#include <format>
#include <cmath>

double juliandate(int j, int m, int a, int hh, int mm, int ss)
{
    if (m==1 || m==2)
    {
        a = a-1;
        m = m+12;
    }
    int c = std::trunc(a/100);
    int b = 2-c+std::trunc(c/4);
    double T = hh/24.0 + mm/1440.0 + ss/86400.0;
    double JD = std::trunc(365.25*(a+4716)) + std::trunc(30.6001*(m+1))+j+b-1524.5; 
    return JD+T;
}

//Constructor (long et lat en °)
Ducktime::Ducktime(double longitudec,double latitudec)
    : longitude(longitudec), latitude(latitudec) //J'ai passé 1h à débugué cette ligne parce que j'avais déclaré la latitude avant la longitude dans Ducktime.h ........ niksamere
{
    std::cout << "Construction Ducktime\n";
    std::cout << std::format("\tLieu d'observation : long {}° / lat {}°\n",longitude,latitude);
    std::cout << '\t';
    reset_time(); //Reset du temps pour mettre à l'heure de l'ordi lors de l'initialisation
}

//Destructor
Ducktime::~Ducktime()
{
    std::cout << "Destructing Ducktime\n";
}

//Date en Jour Julien
double Ducktime::get_JJ() const
{return JJ;}

//Nbr siècles depuis J2000 en JJ
double Ducktime::get_Tu() const
{return Tu;}

//Nbr années depuis J2000 en JJ
double Ducktime::get_Ta() const
{return Ta;}

//Nbr jours depuis J2000 en JJ
double Ducktime::get_Tj() const
{return Tj;}

//On rajoute à JJ le pas de temps dt(qui est en seconde) en Jour
void Ducktime::update_time(double dt)
{
    JJ = JJ+(dt/86400.0);
    Tu = (JJ-2451545)/36525.0;
    Ta = (JJ-2451545)/365.25;
    Tj = (JJ-2451545);
}

//Reset de tous les attributs Ducktime à l'heure de l'ordi
void Ducktime::reset_time()
{
    time_t current_time = time(nullptr);
    tm* utc_time = gmtime(&current_time);
    int a = 1900 + utc_time->tm_year;
    int m = 1 + utc_time->tm_mon;
    int j = utc_time->tm_mday;
    int hh = utc_time->tm_hour;
    int mm = utc_time->tm_min;
    int ss = utc_time->tm_sec;
    std::cout << std::format("Reset Ducktime {}/{}/{} {}:{}:{}\n",j,m,a,hh,mm,ss);

    JJ = juliandate(j,m,a,hh,mm,ss);
    Tu = (JJ-2451545)/36525.0;
    Ta = (JJ-2451545)/365.25;
    Tj = (JJ-2451545);
}

//Update de l'angle horaire du lieu d'observation (définition IAU2000)
void Ducktime::update_Hs() //Normalement il faut UT1 plutôt que UTC en heure mais UTC-UT1 < 1s donc flemme
{
    //Earth Rotation Angle (normalement avec UT1 mais jsp comment l'avoir facilement)
    double ERA = 1296000*(0.7790572732640 + 1.00273781191135448*Tj); //ERA en " --> Source : App new concepts and def (ICRS, CIP, CEO) equation (11)
    
    //Greenwich Mean Sideral Time en "
    double GMST = ERA + 0.014506 + 4612.156534*Tu
    + 1.3915817*pow(Tu,2) - 0.00000044*pow(Tu,3)
    - 0.000029956*pow(Tu,4) - 0.0000000368*pow(Tu,5); //Source : Expres for IAU 2000 precession quantities equation (41)
    
    Hs = (GMST/3600.0+longitude)*M_PI/180.0; //Donne l'heure sidéral (temps local) en radians
}

//Get l'heure sidérale en mémoire (ne pas oublier d'update si besoin avant)
double Ducktime::get_Hs() const
{return Hs;}

double Ducktime::get_latitude_rad() const
{return latitude*M_PI/180.0;}