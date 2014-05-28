//
//  testingBP.cpp
//  
//
//  Created by Hesam Ipakchi on 05/03/2014.
//
//

#include "bm.h"
#include "utility.h"
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sstream>
#include <getopt.h>

int main(int argc, char** argv)
{
    int time_conv = 100, infer_mode = 1, init_flag = 1, LARGE_DEGREE = 50, randseed = 0, vflag = 1;
    int Q = 4;
    double dumping_rate = 1.0, bp_conv_crit = 1.0e-3;
    
    randseed = int(time(NULL));
    
    vector<double> pinVector;
//    pinVector.push_back(0.001);
    pinVector.push_back(0.01);
//    pinVector.push_back(0.1);
//    pinVector.push_back(0.5);
//    pinVector.push_back(0.9);
    
    int sizePinVector = (int) pinVector.size();
    
    for (int i = 0; i < sizePinVector; i++) {
        
        int N = 10000;
        double pin = pinVector[i];
    
        string outputOverlapFilename = "overlapResultsFilesInference/bpOverlapResults_N_" + convertIntToString(N) + "_pin_" + convertDoubleToString(pin) + "_q_" + convertIntToString(Q) + ".dat";
    
        int maxNoIterations = 100;
        
        for (int iteration = 1; iteration <= maxNoIterations; iteration++) {
            double pout = pin * (double) iteration / (double) maxNoIterations;
            cout << "pin = " << pin << ", iteration = " << iteration << ", pout = " << pout << endl;
            double cin = N * pin;
            double cout = N * pout;
            double epsilon = cout / cin;
            
            vector<double> pa, cab;
            for (int q = 0; q < Q-1; q++) {
                pa.push_back(1.0/Q);
            }
            for (int r = 0; r < Q; r++) {
                for (int c = 0; c < Q; c++) {
                    if (r == c) {
                        cab.push_back(cin);
                    }
                    else {
                        cab.push_back(cout);
                    }
                }
            }
            
            blockmodel bm;
            
            bm.LARGE_DEGREE = LARGE_DEGREE;
            bm.init_random_number_generator(randseed);
            bm.set_Q(Q);
            bm.set_vflag(vflag);
            bm.N = N;
            
            string lfnamegengraph ="adjacencyMatrixGMLFiles/pin_" + convertDoubleToString(pin) + "_iteration_" + convertIntToString(iteration) + "_q_" + convertIntToString(Q) + "_n_" + convertIntToString(N) + ".gml";
            
            bm.graph_gen_graph(N, pa, cab);
            
            bm.graph_write_gml(lfnamegengraph.c_str());
            
            string lfname = "adjacencyMatrixGMLFiles/pin_" + convertDoubleToString(pin) + "_iteration_" + convertIntToString(iteration) + "_q_" + convertIntToString(Q) + ".gml";
            assert(!lfname.empty());
            
            
//            bm.graph_read_graph(lfname);
            bm.graph_read_graph(lfnamegengraph);
            
            bm.bm_allocate();
            
            //bm.bm_init_random();
            bm.bm_init(pa,cab);
            
            bm.do_inference(infer_mode, init_flag, bp_conv_crit, time_conv, dumping_rate);
            
            bm.outputOverlap(outputOverlapFilename,epsilon,Q);
            
            pa.clear();
            cab.clear();
        }
    }
    
    pinVector.clear();
    
}