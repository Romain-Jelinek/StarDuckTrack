#pragma once

class Ducktime
{
private :
    double JJ; //Temps en jour julien à l'instanciation de l'objet
    double Tu; //Nbr siècles écoulés depuis J2000 en JJ
    double Ta; //Nbr années écoulées depuis J2000 en JJ
    double Tj; //Nbr jours écoulées depuis J2000 en JJ
    
    const double longitude; //Longitude du lieu d'observation en °
    const double latitude;  //latitude du lieu d'observation en °
    double Hs;              //Heure sidérale locale (aka position du lieu d'obs par rapport au temps)

public :
    //Constructor (long et lat en °)
    Ducktime(double longitude, double latitude);

    //Destructor
    ~Ducktime();
    
    double get_JJ() const;
    double get_Tu() const;
    double get_Ta() const;
    double get_Tj() const;

    void update_time(double dt);    //Rajoute dt secondes à JJ,Tu,Ta,Tj
    void reset_time();              //Réinitialise Ducktime à l'heure de l'ordi

    //Heure sidérale locale -> Angle horaire du lieu d'observation par rapport au point vernale de la date JJ
    void update_Hs();               //Calcul de l'heure sidérale local au temps JJ
    double get_Hs() const;
    double get_latitude_rad() const;
};
