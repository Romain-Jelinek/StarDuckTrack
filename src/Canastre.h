#pragma once
#include <string>
#include "Ducktime.h"

class Canastre
{
private: //Tous les attributs ici en privé
    //Ce dont on a besoin dans le constructor :
    const int ADH;
    const int ADM;
    const double ADS;
    const double CorAD;
    const bool SignDEC; //True = + et False = -
    const int DECD;
    const int DECM;
    const double DECS;
    const double CorDEC;
    const double VMAG;
    const std::string ID;

    //Ce qui est calculé par la suite :

    double AD_bias;     //Prise en compte mouvement propre
    double DEC_bias;

    double AD_date;     //Prise en compte de la précession
    double DEC_date;

    //Normalement je devrais rajouter AD et DEC prenant en compte
    //la nutation ici mais j'ai la flemme

    double Hs;          //Heure sidérale locale
    double Az;          //Azimut locale de l'astre
    double h_vrai;      //Hauteur locale vraie de l'astre (sans atmosphère)
    double h_app;       //Hauteur locale apparente de l'astre ()

public: //Toutes les méthodes ici --> On évite les "setter" pour pas pouvoir modifier directement les attributs privés
    //Constructor
    Canastre(int ADH, int ADM, double ADS, double CorAD,
        bool SignDEC, int DECD, int DECM, double DECS,
        double CorDEC, double VMAG, std::string ID
    );

    //Destructor
    ~Canastre();
    
    //Methods
    void update_eqbias(const Ducktime& ducktime); //Prise en compte du mouvement propre de l'astre (CorAD, CorDEC)
    void update_eqdate(const Ducktime& ducktime); //Prise en compte de la précession des équinoxes selon le modèle IAU1976
    void update_coordlocales_vrai(const Ducktime& ducktime); //Transformation des coordonnées équatoriales à la date vers coordonnées locales avec Hs et la latitude pris depuis l'objet Ducktime
    void update_coordlocales_app(const Ducktime& ducktime,const int pression, const int temperature); //Coordonnées locales apparentes (prise en compte réfraction atmos)

    std::string getstr_ADJ2000_HMS() const;
    std::string getstr_eqbias_rad() const;
    std::string getstr_eqbias_deg() const;
    std::string getstr_eqdate_rad() const;
    std::string getstr_eqdate_deg() const;
    std::string getstr_coordlocales_vrai() const;
    std::string getstr_coordlocales_app() const;

    double get_Az() const;
    double get_happ() const;
};