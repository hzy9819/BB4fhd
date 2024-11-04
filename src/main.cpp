#include "hypergraph.h"
#include "preprocessing.cpp"
#include "td.h"
#include "utils.cpp"
#include <filesystem>

using namespace std;

void * worker(void * s) {
    FILE * fres;
    string file_name((char *) s);
#ifdef HLOG
    // if(0 != access(res_dir, 0)) {
    //     mkdir(res_dir, S_IRWXO);
    // }
    // string res_dir =  std::string(result_prefix) + std::string(ub) + std::string(lb) + std::to_string(TIMEOUTSEC);
    filesystem::path basePath(result_prefix);
    // filesystem::path dirPath(std::string(ub) + std::string(lb) + std::to_string(TIMEOUTSEC));

    // string res_dir = "../../result/sample";
    fres = fopen((basePath / filesystem::path(file_name + ".res")).c_str(), "w");
#ifdef OUTPUTTD
    FILE * ftd = fopen((basePath / filesystem::path(file_name + ".td")).c_str(), "w");
#endif
#else
    fres = stdout;
#endif
    clock_t begin = clock();
    printf("%s: Start Processing\n", file_name.c_str());
    map<size_t, string> vname;
    HyperG H = BuildHyperGraph((string(data_dir) + "/" + file_name).c_str(), vname);

    if(H.N > MAXBITNUMBER) {
        printf("%s: too large to load!\n", file_name.c_str());
        return 0;
    }

    vector <HyperG> HV;

#ifdef PREPROCESS
    HyperG H_old = H;
    Order prefix_o;
    map<size_t, size_t> Vres_map;
    for(size_t i = 0; i < H.N; ++i)
        Vres_map[i] = i;
    Preprocessing(H, prefix_o, Vres_map);
    fprintf(fres, "prefix_o: ");
    for(auto oi: prefix_o)
        fprintf(fres, "%lu ", oi);
    fprintf(fres, "\n");
#ifdef DIVBICCP
    HV = DivBiCCP(H);
    // printf("%s: After preprocessing, %lu BICCP\n", file_name.c_str(), HV.size());
    fprintf(fres, "BICCP_num: %lu\n", HV.size());
#else
    HV.push_back(H);
#endif
#else
    HV.push_back(H);
#endif
    db fhw = 0.;
    for(size_t i = 0; i < HV.size(); ++i) {
        H = HV[i];
        printf("%s: %lu BICCP: n = %lu, m = %lu\n", file_name.c_str(), i, H.N, H.M);
        fprintf(fres, "%lu BICCP: n = %lu, m = %lu\n", i, H.N, H.M);
        if(H.N <= 1 || H.M <= 1) {
            fprintf(fres, "%lu BICCP: width = 1.0\n", i);
            fhw = max(fhw, 1.);
            continue;
        }
        if(H.N > MAXELENUM) {
            fprintf(fres, "%lu BICCP: too large!\n", i);
            return 0;
        }

        // VertexSet C = MaximalClique(H);
        // fprintf(fres, "find maximal clique size: %lu\n", C.size());
        
        // return 0;

#ifdef DPOD
        Order elim_o;
#ifdef BnB
        db ans = DPFHDBB(H, fres, elim_o);
#else
        db ans = DPFHD(H, fres, elim_o);
#endif
        fprintf(fres, "%lu BICCP: width = %lf\n", i, ans);    
        fhw = max(fhw, ans);
#endif

#ifdef ENUM
        H.PrimalG();
        vector <VertexSet> E = H.e;
        vector <size_t> V;
        for(size_t i = 0; i < H.N; ++i) {
            V.push_back(i);
        }
        Width current_ans(0), optimal_ans(inf);
        EnumOrderFHD(H, V, current_ans, optimal_ans, E, 0);
#endif

#ifdef CHECKF
        for(int i = 10; i >= 3 ; --i) {
            Hstate S = make_pair(0, 0);
            optimal_ans = Width(0.5 * i + eps * 2);
            DivOrderFHD(H, S, V, optimal_ans, E, true, clock());
            if(optimal_ans.size())
                printf("%s: check on width %f success\n", file_name.c_str(), 0.5 * i);
            else {
                printf("%s: failed check on width %f\n", file_name.c_str(), 0.5 * i);
                break;
            }
        }
#endif

#ifdef DVO
        Hstate S = make_pair(0, 0);
        DivOrderFHD(H, S, V, optimal_ans, E, false);
        printf("%lf\n", optimal_ans.width);
#endif 

#ifdef CALC_WIDTH
        calc_width(H, optimal_ans.order, E);
#endif
        // printf("%lf\n", optimal_ans.width);
#ifdef PREPROCESS
        for(size_t i = 0; i < elim_o.size(); ++i)
            elim_o[i] = Vres_map[elim_o[i]];
        elim_o.insert(elim_o.begin(), prefix_o.begin(), prefix_o.end());
        H = H_old;
#endif
#ifdef OUTPUTTD
        FHD FT(H, elim_o); 
        // fprintf(fres, "elimination order: ");
        // for(size_t i = 0; i < elim_o.size(); ++i)
        //     fprintf(fres, "%lu ", elim_o[i]);
        // fprintf(fres, "\n");
        FT.Refine();
        fprintf(ftd, "%lu\n", FT.X.size());
        for(auto Xi: FT.X) {
            vector <size_t> tmpV;
            Xi.getelement(tmpV);
            for(auto v: tmpV)
                fprintf(ftd, "%s ", vname[v].c_str());
            fprintf(ftd, "\n");
        }
        for(auto ei: FT.eg) 
            fprintf(ftd, "%lu %lu\n", ei.first, ei.second);
#endif
    }

    printf("%s: Finish\n", file_name.c_str());
    fprintf(fres, "fhw = %lf\n", fhw);
    clock_t end = clock();
    fprintf(fres, "total_time = %lf ms\n", 1000. *  (end - begin) / CLOCKS_PER_SEC);
    return 0;
// #endif
}

int main(int argc, const char * argv[]) {
    vector <string> file_names ;

#ifdef IDV_TEST
    ifstream fin;
    string str;
    fin.open(idv_file, ios::in);
    while(getline(fin, str))
        file_names.push_back(str.c_str());
    // file_names.push_back("rand-15-23-3-16-150000-23.xml.hg");
#else
    file_names = data_loader(data_dir, "full");
#endif

    // string dir(data_dir);
    // string dir(".");
    // thread_queue Q;
    // for(auto it = file_names.begin(); it != file_names.end(); it++) {
    //     pthread_t * tid = Q.alloc();
    //     while(tid == NULL) {
    //         sleep(5);
    //         tid = Q.alloc();
    //     }
    //     int ret = pthread_create(tid, NULL, worker, (void *) (*it).c_str());
    //     if(ret != 0) {
    //         cerr << "pthread_create error: error_code=" << ret << endl;
    //     }
    // }

    // while(!Q.finish())
    //     sleep(5);
#ifdef MULTITHREAD
    for(auto it = file_names.begin(); it != file_names.end(); it++)
        worker((void *) (*it).c_str());
#endif

#ifdef MULTIPROGRESS
    size_t curr_worker = 0;
    for(auto it = file_names.begin(); it != file_names.end(); it++)
        if(curr_worker >= worker_num) {
            int statues = 0;
            wait(&statues);
            curr_worker--;
            it--;
        }
        else {
            curr_worker++;
            pid_t pid = fork();
            if(pid < 0) {
                printf("fork error!\n");
                return -1;
            }
            else if(pid == 0) {
                worker((void *) (*it).c_str());
                return 0;
            }
        }
    while(curr_worker > 0) {
        int statues = 0;
        wait(&statues);
        curr_worker--;
    }
#else
    for(auto it = file_names.begin(); it != file_names.end(); it++)
        worker((void *) (*it).c_str());
#endif
    return 0;
}