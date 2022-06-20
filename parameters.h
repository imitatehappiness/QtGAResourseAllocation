#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <fstream>

extern  int     MAX_POP;
extern  int     MAX_STUFF;
extern  int     MAX_TASK;
extern  double  MAX_TOTAL_TIME;
extern  int     MAX_SOLUTION;
extern  float   EPS;
extern  float   pc;
extern  float   pm;
extern  int     count_skills;

extern std::ofstream fout;//("result.txt");

struct skill{
    int *arr_skill;
    int rang;
    skill(){
        arr_skill = new int[count_skills];
    }


    skill(skill& obj){
        arr_skill = new int[count_skills];
        rang  = obj.rang;
        for (int i = 0; i < count_skills; i++)
            arr_skill[i] = obj.arr_skill[i];

    }

    skill & operator=(const skill & obj) {
        if (this != &obj) {
            delete[] arr_skill;
            arr_skill = new int[count_skills];
            rang  = obj.rang;
            for (int i = 0; i < count_skills; i++)
                arr_skill[i] = obj.arr_skill[i];
        }
        return *this;
    }

    ~skill(){
        delete[] arr_skill;
    }

};

struct Gen{
    double time = 0;
    int *number;
    Gen(){
        number = new int[MAX_STUFF];  
    }

    Gen(Gen& obj){
        number = new int[MAX_STUFF];
        time = obj.time;
        for (int i = 0; i < MAX_STUFF; i++)
            number[i] = obj.number[i];
    }

    Gen & operator=(const Gen & obj) {
        if (this != &obj) {
            time = obj.time;
            delete[] number;
            number = new int[MAX_STUFF];
            for (int i = 0; i < MAX_STUFF; i++)
                number[i] = obj.number[i];
        }
        return *this;
    }

    ~Gen(){
        delete[] number;
    }
};

struct Chromosome{ //популяция задач

    Gen *alleles;
    double fitness = 0;
    double total_time = 0;
    double fitness_skill = 0;
    Chromosome(){
        alleles = new Gen[MAX_TASK];
    }

    Chromosome(Chromosome& obj){
        alleles = new Gen[MAX_TASK];
        fitness = obj.fitness;
        fitness_skill = obj.fitness_skill;
        total_time = obj.total_time;
        for (int i = 0; i < MAX_TASK; i++)
            alleles[i] = obj.alleles[i];
    }

    Chromosome & operator=(const Chromosome & obj) {
        if (this != &obj) {

            delete[] alleles;
            fitness = obj.fitness;
            fitness_skill = obj.fitness_skill;
            total_time = obj.total_time;
            alleles = new Gen[MAX_TASK];
            for (int i = 0; i < MAX_TASK; i++) {
                alleles[i] = obj.alleles[i];
            }
        }
        return *this;
    }

    ~Chromosome(){
        delete[] alleles;
    }

};


struct twr{
    int num_task;
    int num_worker;
    double resourse;

};

extern Chromosome *population;
extern Chromosome *parents;
extern skill *task_s;
extern skill *stuff_s;
extern twr* arr_t;

#endif // PARAMETERS_H
