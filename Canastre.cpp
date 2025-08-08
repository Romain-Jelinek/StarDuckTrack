#include "Canastre.h"
#include "Ducktime.h"
#include <iostream>
#include <string>
#include <format>
#include <cmath>

double DMS2arcsec(bool Sign,int D, int M, double S)
{
    if (Sign){
        return D*3600 + M*60.0 + S;}
    else {
        return -(D*3600 + M*60.0 + S);}
}

double HMS2arcsec(int H, int M, double S)
{return (H*3600 + M*60 + S)*15;}

double arcsec2rad(double arcsec)
{return arcsec*M_PI/648000.0;}

double rad2deg(double rad)
{return rad*180/M_PI;}

// ________________________________________________________________________
//Constructor
Canastre::Canastre(int ADHc, int ADMc, double ADSc, double CorADc,
            bool SignDECc, int DECDc, int DECMc, double DECSc,
            double CorDECc, double VMAGc, std::string IDc) :
            ADH(ADHc), ADM(ADMc), ADS(ADSc), CorAD(CorADc),
            SignDEC(SignDECc), DECD(DECDc), DECM(DECMc), DECS(DECSc),
            CorDEC(CorDECc), VMAG(VMAGc),ID(IDc) 
{
    char strSignDEC = '-';
    if (SignDECc) {strSignDEC = '+';}
    std::cout << std::format("Constructing {} :\n\tAD : {}H {}m {}s\n\tDEC: {}{}° {}' {}\"\n\n",ID,ADH,ADM,ADS,strSignDEC,DECD,DECM,DECS);
}

//Destructor
Canastre::~Canastre()
{
    std::cout << std::format("Destructing {}\n",ID);
}

// ________________________________________________________________________
//Toutes les fonctions d'update

//On prend en compte le mouvement propre des astres depuis l'epoch J2000.0 avec update_eqbias()
void Canastre::update_eqbias(const Ducktime& ducktime) //Ta issue de Ducktime.get_Ta()
{
    double Ta = ducktime.get_Ta();
    AD_bias = arcsec2rad(HMS2arcsec(ADH,ADM,ADS) + CorAD*Ta);
    DEC_bias = arcsec2rad(DMS2arcsec(SignDEC,DECD,DECM,DECS) + CorDEC*Ta);
}

//On prend en compte la précession des équinoxes depuis l'epoch J2000.0 avec update_eqdate()
void Canastre::update_eqdate(const Ducktime& ducktime)
{
    //On commence par prendre en compte le mouvement propre
    update_eqbias(ducktime);

    //Temps écoulé depuis J2000 en siècle julien
    double Tu = ducktime.get_Tu(); 

    //Calcul des angles de précession à la date depuis J2000.0
    double zeta  = arcsec2rad(2306.2181 * Tu + 0.30188 * pow(Tu, 2) + 0.017998 * pow(Tu, 3));
    double z  = arcsec2rad(2306.2181 * Tu + 1.09468 * pow(Tu, 2) + 0.018203 * pow(Tu, 3));
    double theta = arcsec2rad(2004.3109 * Tu - 0.42665 * pow(Tu, 2) - 0.041833 * pow(Tu, 3));

    // Pré-calculs trigonométriques pour gagner du temps
    double sz = sin(z);
    double cz = cos(z);
    double stheta = sin(theta);
    double ctheta = cos(theta);
    double cDec0 = cos(DEC_bias);
    double sDec0 = sin(DEC_bias);
    double cADzeta = cos(AD_bias + zeta);
    double sADzeta = sin(AD_bias + zeta);

    //Calcul coordonnées équatoriales à la date JD
    //Les sources de ce calcule c'est moi. J'ai repris la matrice de passage de
    //"Precession Matrix Based on IAU (1976) System of Astronomical Constants"
    //et j'ai retravaillé les équat° pour resortir AD et DEC à la date d'observation
    //Déclinaison de l'astre à la date d'observation
    DEC_date = asin(cDec0 * stheta * cADzeta + ctheta * sDec0);

    //Ascension Droite de l'astre à la date d'observation
    double up = cDec0 * (sz * ctheta * cADzeta + cz * sADzeta) - (sDec0 * sz * stheta);
    double down = cDec0 * (cz * ctheta * cADzeta - sz * sADzeta) - (sDec0 * cz * stheta);
    AD_date = fmod((atan2(up, down) + 2*M_PI), 2*M_PI); // modulo 2π
}

//Calcul des coordonnées locales Az/h_vrai (sans atmosphère)
void Canastre::update_coordlocales_vrai(const Ducktime& ducktime)
{
    //On commence par update les coordonnées equatoriales à la date exacte
    //update_eqdate(); //j'ai commenté pour laisser le choix dans le main d'update ou pas
    
    //Heure locales de l'astre
    double Hl = ducktime.get_Hs()-AD_date; //Tout est en radians

    // Pré-calcule les trigonométries pour optimiser
    double cDec = cos(DEC_date);
    double sDec = sin(DEC_date);
    double clat = cos(ducktime.get_latitude_rad());
    double slat = sin(ducktime.get_latitude_rad());
    double cHl = cos(Hl);

    // Calcul de hauteur
    h_vrai = rad2deg(asin(clat * cHl * cDec + slat * sDec));

    // Calcul de l’azimut
    Az = rad2deg(fmod(atan2(cDec * sin(Hl), -clat * sDec + slat * cDec * cHl) + M_PI, 2*M_PI));
}

//Calcul des coordonnées locales Az/h_app (réfraction atmosphérique)
void Canastre::update_coordlocales_app(const Ducktime& ducktime,const int pression, const int temperature)
{
    update_coordlocales_vrai(ducktime); //Pour avoir Az et h_vrai

    //Calcule la réfraction atmosphérique en minutes d'arc selon le modèle de Bennett/Sæmundsson.
    //Ne s'applique que pour les h > -5°, parce que osef de ce qu'il y a sous l'horizon et ça évite les / par 0
    double R = 0;
    if (h_vrai > -5){
        // Bennett (1982) en minutes d'arc
        double R_bennett = 1.02 / tan((M_PI/180.0)*(h_vrai + 10.3 / (h_vrai + 5.11)));
        
        //Correction pression/température
        double correction = (pression / 1010.0) * (283.0 / (273.0 + temperature));
        R = (R_bennett * correction)/60; // R en ° comme ça on l'ajoute juste à h_vrai pour avoir h_app
    }

    //Hauteur apparante de l'astre
    h_app = h_vrai + R;
}


// ________________________________________________________________________
//Les fonctions getstr

std::string Canastre::getstr_ADJ2000_HMS() const
{
    return std::format("AD : {}H {}m {}s\n", ADH,ADM,ADS);
}

std::string Canastre::getstr_eqbias_rad() const
{
    return std::format("AD_bias : {} rad / DEC_bias : {} rad\n",AD_bias,DEC_bias);
}

std::string Canastre::getstr_eqbias_deg() const
{
    return std::format("AD_bias : {}° / DEC_bias : {}°\n",rad2deg(AD_bias),rad2deg(DEC_bias));
}

std::string Canastre::getstr_eqdate_rad() const
{
    return std::format("AD_date : {} rad / DEC_date : {} rad\n",AD_date,DEC_date);
}

std::string Canastre::getstr_eqdate_deg() const
{
    return std::format("AD_date : {}° / DEC_date : {}°\n",rad2deg(AD_date),rad2deg(DEC_date));
}

std::string Canastre::getstr_coordlocales_vrai() const
{
    return std::format("Az : {}° / h_vrai : {}°\n",Az,h_vrai);
}

std::string Canastre::getstr_coordlocales_app() const
{
    return std::format("Az : {}° / h_app : {}°\n",Az,h_app);
}

//Retourne la valeur brut d'Az en °
double Canastre::get_Az() const
{
    return Az;
}

//Retourne la valeur brut d'h_app en °
double Canastre::get_happ() const
{
    return h_app;
}
