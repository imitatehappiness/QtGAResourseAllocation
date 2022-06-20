#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <QMainWindow>
#include <QDebug>
#include <parameters.h>
#include <fstream>


namespace Ui {
class GeneticAlgorithm;
}

class GeneticAlgorithm : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeneticAlgorithm(QWidget *parent = 0);
    ~GeneticAlgorithm();

    void get_res(int,int,int,int,int,float,float,float);
    Chromosome GA_Launch();
    Chromosome Solve();
    void Crossing_mutation(Chromosome *parents, int count_parents, int count_while);
    void New_population_set(Chromosome *population_and_children, int new_max_pop);
    void Sort(Chromosome *A, int N);
    void Selection(double *selection, Chromosome *parents, int count_parents);
    void fitness_function();
    void Initial_population();
    double Delta(double, int, float);
    void Print_chromosome(Chromosome);
    bool check_result(Chromosome);
    Chromosome Compare_Chromosome(Chromosome, Chromosome);
signals:
    void s_progress(int);
private:

    Ui::GeneticAlgorithm *ui;
};

#endif // GENETICALGORITHM_H
