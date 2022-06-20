#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPixmap>
#include <time.h>
#include <QDebug>
#include <parameters.h>
#include <QFile>
#include <fstream>
#include <math.h>
#include <qcustomplot.h>
#include <QMessageBox>
#include <QVector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Chromosome GA_Launch();
    Chromosome Solve(int progress);
    void Crossing_mutation(Chromosome *parents, int count_parents, int count_while);
    void New_population_set(Chromosome *population_and_children, int new_max_pop);
    void Sort(Chromosome *A, int N);
    void Selection(double *selection, Chromosome *parents, int count_parents);
    void fitness_function();
    void fitness_skill_function();
    void Initial_population();
    double Delta(double, int, float);
    void Print_chromosome(Chromosome);
    bool check_result(Chromosome);
    Chromosome Compare_Chromosome(Chromosome, Chromosome);
    void DrawGr();
    void DrawGr2();
    void clear_all_btn();
    void open_menu();
    void close_menu();
    QVector<int> get_task(int);

    QCustomPlot *wG;
    QCustomPlot *wG2;

private:
   private slots:
    void on_set_value_clicked();
    void on_START_clicked();
    void on_cleare_table_clicked();
    void on_Random_stuff_clicked();
    void on_Random_task_clicked();
    void on_set_skill_task_clicked();
    void on_set_skill_stuff_clicked();
    void on_go_to_setting_page_clicked();
    void on_go_to_skill_task_page_clicked();
    void on_go_to_skill_stuff_page_clicked();
    void on_go_to_gr_page_clicked();
    void on_MENU_clicked();
    void on_HOME_clicked();
    void on_go_to_phone_page_clicked();
    void on_go_to_gr_page_2_clicked();
    void on_zooming_clicked();
    void on_zooming_2_clicked();
    void on_draw_gr2_clicked();

    void on_set_par_clicked();

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
