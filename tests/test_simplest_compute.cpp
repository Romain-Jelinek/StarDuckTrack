#include <iostream>
#include <string>
#include <format>
#include "Canastre.h"
#include "Ducktime.h"

/*
    Test le plus simple possible
*/

int main(){

    int ADH = 18;
    int ADM = 36;
    double ADS = 56.336939;
    double CorAD = 0.2587501997522312;
    bool SignDEC = true;
    int DECD = 38;
    int DECM = 47;
    double DECS = 1.28333;
    double CorDEC = 0.28667;
    double VMAG = 0.03;
    std::string ID = "HR 7001";

    double longitude = 1.4436694444444444;
    double lattitude = 43.604258333333334;
    int temperature = 10;
    int pression = 1010;

    Canastre Vega(
        ADH, ADM, ADS, CorAD,
        SignDEC, DECD, DECM, DECS,
        CorDEC, VMAG, ID
    );

    Ducktime ducktime(longitude, lattitude);

    Vega.compute_current_local_app(ducktime, pression, temperature);

    std::cout << std::format("\n\t|_{} current local coord : {}",Vega.getID(),Vega.getstr_coordlocales_app());

    return 0;
}