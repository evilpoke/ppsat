//
// Created by Ning Luo on 5/18/21.
//

#include <math.h>
#include "formula.hpp"
#include <memory>
#include <chrono>
#include <exception>
#include <stdlib.h>
#include <ctime>
#include "clause.hpp"
#include "literal.hpp"
#include "model.hpp"
#include "utils.hpp"
#include "solver.hpp"
#include "emp-sh2pc/emp-sh2pc.h"
#include <emp-tool/emp-tool.h>
#include "emp-ag2pc/emp-ag2pc.h"  // not required though

using namespace std;
using namespace emp;


int main(int argc, char** argv) {

    try {
        // Generating the circuit file with dummy input
        // This file can then be used to perform a ppSAT execution 

        
        //symmetric_ppsat_circuit.circuit.txt is prepared to be written to 
        setup_plain_prot(true, "symmetric_ppsat_circuit.circuit.txt");
        cout << "Generated an initial circuit template..." << endl;
        
        // There are the number of parameters which are fixed after the circuit generation
        int number_of_steps = atoi(argv[3]);
        int nvar = atoi(argv[4]);

        //std::string dummy1 = "(";
        std::stringstream dummy1;

        for (int d = 0; d < 3; d++){
            //looping through 3 clauses
            dummy1 << "(";
            for (int i = 0; i < nvar; i++){
                //looping through the number of variables
                dummy1 << i;
                if (i < nvar){
                    dummy1 << " ";
                }
            }
            dummy1 << ")";
        }

        std::stringstream dummy2;
        for (int d = 0; d < 3; d++){
            //looping through 3 clauses
            dummy2 << "(";
            for (int i = 0; i < nvar; i++){
                //looping through the number of variables
                dummy2 << "-";
                dummy2 << i;
                if (i < nvar){
                    dummy2 << " ";
                }
            }
            dummy2 << ")";
        }
        std::string strdummy1 = dummy1.str(); 
        std::string strdummy2 = dummy2.str(); 
        cout << "Created dummy strings for formulars..." << endl;

        auto phi_a = make_unique<Formula>(nvar, strdummy1, ALICE);
        auto phi_b = make_unique<Formula>(nvar, strdummy2, BOB);
        cout << "Created dummy Formular cpp objects..." << endl;
        auto phi = phi_a -> conjunction(phi_b);
        cout << "Conjugated the two objects..." << endl;
        
        Solver solver(nvar, phi);
        cout << "Created the solver object." << endl;

        auto model = solver.solve(number_of_steps, false);
        cout << "Template: Model would then be printed when circuit is used :\n"; 
        cout << model->toString() << endl; 
        
    }

    catch (char const *msg) {
        cout << "Main catch msg: " << msg << endl;
    }
    catch (const exception &e) {
        cout << "Main cat exception: " << e.what() << endl;
    }


}



