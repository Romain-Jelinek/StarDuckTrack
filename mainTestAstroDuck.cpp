/*
    Main pour tester la classe Canastre
*/

#include <iostream>
#include <string>
#include <format>
#include "Canastre.h"
#include "Ducktime.h"

int main(){
    //Test Constructor Canastre
    Canastre Vega(
        18, 36, 56.336939, 0.2587501997522312,
        true, 38, 47, 1.28333, 0.28667,
        0.03, "HR 7001"
    );
    std::cout << ".getstr_ADJ2000_HMS() : " << Vega.getstr_ADJ2000_HMS();

    //Test Constructor Ducktime et ses méthodes
    Ducktime Testtime(1.4436694444444444,43.604258333333334);
    std::cout << std::format("\n.get_JJ() : {}\n",Testtime.get_JJ());
    std::cout << std::format(".get_Tu() : {}\n",Testtime.get_Tu());
    std::cout << std::format(".get_Ta() : {}\n",Testtime.get_Ta());
    std::cout << std::format(".get_Tj() : {}\n",Testtime.get_Tj());

    std::cout << "\nUn jour plus tard... (.update_time(86400))\n";
    Testtime.update_time(86400);
    std::cout << std::format(".get_JJ() : {}\n",Testtime.get_JJ());
    std::cout << std::format(".get_Tu() : {}\n",Testtime.get_Tu());
    std::cout << std::format(".get_Ta() : {}\n",Testtime.get_Ta());
    std::cout << std::format(".get_Tj() : {}\n",Testtime.get_Tj());
    
    std::cout << "\nAprès un .reset_time()\n";
    Testtime.reset_time();
    std::cout << std::format(".get_JJ() : {}\n",Testtime.get_JJ());
    std::cout << std::format(".get_Tu() : {}\n",Testtime.get_Tu());
    std::cout << std::format(".get_Ta() : {}\n",Testtime.get_Ta());
    std::cout << std::format(".get_Tj() : {}\n",Testtime.get_Tj());

    Testtime.update_Hs();
    std::cout << std::format(".get_Hs() (après .update_Hs()) : {}\n",Testtime.get_Hs());
    
    //Test d'update des coordonnées de l'astre Canastre
    //Je fais passer l'objet Ducktime direct en argument de la class Canastre !
    //C'est cool non? Comme ça pas d'emmerde dans le main, il peut pas y avoir d'erreur
    // On aura juste à faire un update de l'objet Ducktime dans les boucles de calculs
    // Puis update les coordonnées Canastre
    std::cout << "\nUpdate coordonnées equatoriales avec mouvement propre à la date JJ .update_eqbias()";
    Vega.update_eqbias(Testtime); //Prise en compte mouvement propre de l'astre
    std::cout << std::format("\n.getstr_eqbias_rad() : {}",Vega.getstr_eqbias_rad());
    std::cout << std::format(".getstr_eqbias_deg() : {}",Vega.getstr_eqbias_deg());
    
    std::cout << "\nUpdate coordonnées equatoriales avec precession à la date JJ .update_eqdate()";
    Vega.update_eqdate(Testtime); //Prise en compte précession des equinoxes (modèle IAU1976)
    std::cout << std::format("\n.getstr_eqdate_rad() : {}",Vega.getstr_eqdate_rad());
    std::cout << std::format(".getstr_eqdate_deg() : {}",Vega.getstr_eqdate_deg());
    
    std::cout << "\nUpdate coordonnées locales vrai .update_coordlocales_vrai()";
    Vega.update_coordlocales_vrai(Testtime);
    std::cout << std::format("\n.getstr_coordlocales_vrai() : {}",Vega.getstr_coordlocales_vrai());

    std::cout << "\nUpdate coordonnées locales apparantes .update_coordlocales_app()";
    Vega.update_coordlocales_app(Testtime,1010,10);
    std::cout << std::format("\n.getstr_coordlocales_app() : {}",Vega.getstr_coordlocales_app());
    
    return 0;
}