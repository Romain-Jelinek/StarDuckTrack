# StarDuckTrack

C++ library for the StarDuck project aiming to compute local coordonates (azimut + altitude) from J2000 equatorial coordonates.

## Version
### Current version :

- 1.0.0

    - Initial commit version.

### Version history :

- None

## Requirements

C++ 20 standard library should be enough to compile

## Compilation

Just clone the repo & `make`. Test examples are compiled in `StarDuckTrack/tests` folder.

## Description

The project is separated in two part :
- **Canatre class**, wich is the class describing celestial objects. Their equatorial coordonates are stored in this class and methods are provided to compute their local coordonates, given a Ducktime observator.

- **Ducktime class**, wich is like the "observator" class. It describes the coordonates of the observator on earth (latitude & longitude) but also its related local time. This class can be seen as the "space & time" reference for computation of local coordonates.
    
For now, i use a hybrid model to compute equatorial coordonate to the current date, wich may be wrong... I use the "Precession Matrix Based on IAU (1976) System of Astronomical Constants" precession model and without any nutation correction.

But I use a definition from the IAU2000 model to update the sideral time and without bulletin correction from IERS wich may also be wrong (I use UTC instead of UT1 in the calculation).

A simple model of atmospheric refraction is used to compute the apparent star altitude depending on the local temperature.
