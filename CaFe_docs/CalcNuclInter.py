# calculate proton-nucleus interaction cross sections
# estimate survival probability of proton-hadron interactions thru matter
# sig_total = sig_elastic + sig_inelastic
# for elastic/inelastic cross sections, used the relation: sig = sig0*A^alpha
#(See: "Compilation of cross sections for proton–nucleus interactions at the HERA energy" J. Carvalho 2003)

import sys
import numpy as np

def calc_inelasticXsec(A):
    
    # A: mass number or number of nucleons (protons+neutrons)
    # this function calculates the inelastic p-A cross section
    # parameters obtained from Fig 3 (bottom). of J. Carvalho (2003)
    sig0 = 43.55  # 43.55 +/- 0.40 mb
    alpha = 0.7111 # 0.7111 +/- 0.0011
    sig_inelastic = sig0 * (A**alpha)

    return sig_inelastic # inelastic proton-A cross-section in millibarns (mb)


def calc_totalXsec(A):

    # A: mass number or number of nucleons (protons+neutrons)
    # this function calculates the total p-A cross section
    # parameters obtained from Fig 3 (top). of J. Carvalho (2003)
    sig0 = 52.86  # 52.86 +/- 0.23 mb
    alpha = 0.7694 # 0.7694 +/- 0.0012
    sig_elastic = sig0 * (A**alpha)

    return sig_elastic # elastic proton-A cross-section in millibarns (mb)

def calc_mean_free_path(M, rho, sig):
    # calculate the mean free path of incident particle with an atom (or nucleus)
    # with molar mass M (g/mole), and material density, rho (g/cm3) and collision
    # probability, sig (mb), of the incident particle with a nucleus of the target
    
    N_Av = 6.022e23  # atoms / mole (Avogadro's Number)
    mb2cm = 1e-27 / 1.   # millibarn to cm2 conversion:  1 mb is 1e-27 cm2
    sig = sig * mb2cm

    # mean free path (in cm)
    lam = M / (rho * N_Av * sig)
    return lam  # avg distance incident particle travels thru material before interacting (mean free path) in cm

def calc_Intr_Prob(l, lam):
    # calculates the probability that an incident particle
    # travelling up to a path length l (cm) interacts with the material
    
    Pl = 1. - np.exp(-l/lam)  # probability that a particle has an interaction up to a path length l
    Ps = 1 - Pl # survival probability, or the probability that a particle does not suffer a collision up to a distance l
    return Pl
    
def main():

    #print('Number of arguments:', len(sys.argv), 'arguments.')
    #print('Argument List:', str(sys.argv))

    # For Ca-48 target:
    rho = 1.86 # g/cm3
    M = 47.9525  # g/mole
    Lt = 0.4301  # target length in cm 
    Rt = 0.635   # target radius in cm
    # Mass Number of target atom in material
    A = 48  # int(sys.argv[1])

    
    sig_elastic = calc_totalXsec(A) - calc_inelasticXsec(A)
    sig_inelastic = calc_inelasticXsec(A)

    # calculate mean free path
    lam = calc_mean_free_path(M, rho, sig_inelastic)

    # calculate path travelled by proton at 66.9 deg from +z axis
    l = np.sqrt( (Lt/2)**2 + Rt**2 )  # approximation for now (need to check with Geant4 code to get distance)

    # calculate pA inelastic interaction probability
    Pl = calc_Intr_Prob(l, lam)
    Ps = 1. - Pl
    print('A =', A, '\n p-A elastic cross section (mb): ', sig_elastic,
          '\n p-A inelastic cross section (mb): ', calc_inelasticXsec(A),
          '\n p-A total cross section (mb): ', calc_totalXsec(A),
          '\n ',
          '\nmean free path (cm): ', lam,
          '\nfor a path length travelled (cm): ', l,
          '\n1. interaction prob. with material :', Pl,
          '\n2. survival prob. in material : ', Ps)
    
        
if __name__ == "__main__":
    main()
