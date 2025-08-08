#include <format>
#include <iostream>
#include <chrono>
#include <thread>
#include "Ducktime.h"
#include "Canastre.h"

// ______________________________________________________//
/*
    Exemple de boucle de calcul avec Ducktime et Canastre
*/
// ______________________________________________________//

int main()
{
    double dt = 0.1;        //Pas d'incrémentation du temps en s
    double tobs = 86400;  //Temps d'observation en s

    //On créer un astre Canastre avec toutes ses coordonnées J2000, mouvement propre, magnitude et un ID
    Canastre Vega(
        18, 36, 56.336939, 0.2587501997522312,
        true, 38, 47, 1.28333, 0.28667,
        0.03, "HR 7001"
    );

    //On créer un objet Ducktime avec la longitude et latitude de l'observateur en °
    Ducktime letemps(1.4436694444444444,43.604258333333334); //L'initialisation de Ducktime calcule automatique le Jour Julien à la seconde où on créer l'objet

    std::cout << '\n';
    double t = 0; //On initialise le temps à 0 que l'on incrémentera
    while (t < tobs)
    {
        letemps.update_Hs();            //On commence par update l'heure sidérale (position de l'observateur par rapport au point vernal)
        Vega.update_eqdate(letemps);    //Puis on update les coordonnées équatoriales de l'epoch J2000.0 jusqu'à la date d'observation.
        /*
        En réalité il y'a 2 update qui sont faites implicitement avec update_eqdate().dt :
            1 - Prise en compte du mouvement propre de l'astre avec update_eqbias() inclus dans l'appel de update_eqdate()
            2 - Prise en compte de la précession des équinoxes depuis J2000.0
        */
        Vega.update_coordlocales_app(letemps,1010,10); //Enfin on peut calculer les coordonnées Az et h apparant en ° et en fonction de la pression et de la température
        /*
        Ici aussi il y un appel de 2 update fait implicitement :
            1 - Calcul des coordonnées locales vrai (sans atmosphère) dans un premier temps avec update_coordlocales_vrai() inclus dans update_coordlocales_app()
            2 - Ajustement de la hauteur en prenant en compte la réfraction atmosphèrique
        */
        letemps.update_time(dt); //On update JJ pour la boucle suivante en rajoutant le pas dt
        t += dt;
        
        std::cout << std::format("\rAz {}° / h_app {}° ",Vega.get_Az(),Vega.get_happ()) << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}