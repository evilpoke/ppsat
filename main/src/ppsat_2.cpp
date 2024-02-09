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
using namespace std;
using namespace emp;


unique_ptr<Formula> and_wrapper(int nvar, string phi) {
    //
    //Wrapper for two phis being inputted
    //
	auto phi_a =  make_unique<Formula>(nvar, phi, ALICE);
    auto phi_b =  make_unique<Formula>(nvar, phi, BOB);
	
    // anding the phis
    auto return_phi = phi_a -> conjunction(phi_b);
    return return_phi;
    
}



int main(int argc, char** argv) {

    int port, party;
    parse_party_and_port(argv, &party, &port);
    NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);
    setup_semi_honest(io, party);

    int number_of_steps = atoi(argv[3]);
    int nvar = atoi(argv[4]);
    string inputformular = argv[5];

    auto phi = and_wrapper(nvar, inputformular);

    Solver solver(nvar, phi);
    auto model = solver.solve(number_of_steps, false);
    
    cout << model->toString() << endl;
    delete io;

}



