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
#include "test/single_execution.h"
#include "parser.hpp"
#include "literal.hpp"
#include "formula.hpp"
#include "utils.hpp"

using namespace std;
using namespace emp;

string extract_satset(bool * out){
	//extract the sat model
}

void insert_formular_material_into_memory(bool *in, bool * out, string inputtext,int nvar){
	vector<string> raw_cls = Parser::parse_clauses(inputtext);
	//active = new Bit[raw_cls.size()];
	int i = 0;

	//b0 = b0 | (neg_vars[i] & ell.isIndex(i, 0));
	//b1 = b1 | (pos_vars[i] & ell.isIndex(i, 1));
	int counter = 0;
	for (auto raw_cl : raw_cls) {
		vector<int> vars = Parser::parse_literals(raw_cl);  //&ell raw_cl
		bool* pos_vars = new bool[nvar];
		bool* neg_vars = new bool[nvar];
		for (int i = 0; i < nvar; i++)
		{
			bool has_pos = find(vars.begin(), vars.end(), i) != vars.end();
			bool has_neg = find(vars.begin(), vars.end(), -i) != vars.end();
			neg_vars[i] = has_pos; 
			pos_vars[i] = has_neg;

		}//print(& d_gag) //print(& pos_vars)
		for (int i = 0; i < raw_cl.size(); i++)
		{
			in[counter+i] = pos_vars[i];
		}
		for (int i = 0; i < raw_cl.size(); i++)
		{
			in[counter+i + raw_cl.size()] = neg_vars[i];
		}
		
		counter = counter + raw_cl.size()*2*nvar;

}

template<typename T>
void perform_inputdependent_ppsat(int party, T* io, string name, string check_output = "", string hin = "", int nvar){
    cout << "Reading from circuit file..." <<endl;
    string file = name;
	BristolFormat cf(file.c_str());
	auto t1 = clock_start();
	C2PC<T> twopc(io, party, &cf);
	io->flush();
	cout << "one time:\t"<<party<<"\t" <<time_from(t1)<<endl;

	t1 = clock_start();
	twopc.function_independent();
	io->flush();
	cout << "inde:\t"<<party<<"\t"<<time_from(t1)<<endl;

	t1 = clock_start();
	twopc.function_dependent();
	io->flush();
	cout << "dep:\t"<<party<<"\t"<<time_from(t1)<<endl;

	bool *in = new bool[max(cf.n1, cf.n2)];
	bool * out = new bool[cf.n3];
	cout << "input size: max " << cf.n1 << "\t" << cf.n2 << endl;
    cout << "input for " << party << ": 0" << endl;
    insert_formular_material_into_memory(in, out, hin, nvar);

	//if (party == ALICE) {
	//    memset(in, false, max(cf.n1, cf.n2));
	//} else {
	//    memset(in, false, max(cf.n1, cf.n2));
	//}
	//memset(out, false, cf.n3);

    t1 = clock_start();
	twopc.online(in, out);
	cout << "online:\t"<<party<<"\t"<<time_from(t1)<<endl;

	if(party == BOB and check_output.size() > 0){
        res = extract_satset(out)
		//string res = "";
		//for(int i = 0; i < cf.n3; ++i)
		//	res += (out[i]?"1":"0");
		cout << "result: " << res << endl;
        cout << "expected result (this is hardcoded): " << hex_to_binary(check_output) << endl;
		
	}
	delete[] in;
	delete[] out;


}


int main(int argc, char** argv) {

    try {
        // We now import the generated circuit file and fill the generated memset spaces with 
        // our private formulars of the parties. The circuit file has to be made available
        // to both parties beforehand.
        int port, party;
        parse_party_and_port(argv, &party, &port);
        NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);
        std::string inputfromoneparty = argv[5];
        perform_inputdependent_ppsat(party, io, "symmetric_ppsat_circuit.circuit.txt", "" , inputfromoneparty, argv[6]);
        delete io;
        return 0;
    }

    catch (char const *msg) {
        cout << "Main catch msg: " << msg << endl;
    }
    catch (const exception &e) {
        cout << "Main cat exception: " << e.what() << endl;
    }

}



