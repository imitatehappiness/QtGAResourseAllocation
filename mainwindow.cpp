#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

int     MAX_POP = 0;
int     MAX_STUFF = 0;
int     MAX_TASK = 0;
double  MAX_TOTAL_TIME = 0;
int     MAX_SOLUTION = 0;
float   EPS = 0;
float   pc = 0;
float   pm = 0;
double  *av_t;
int     count_skills = 0;

Chromosome *population = {0};
Chromosome *parents = {0};
skill *task_s = {0};
skill *stuff_s = {0};
QString clk_btn = "QPushButton{background-color: rgb(22, 22, 22);border: solid; color: #EEEEEE;border-left-width: 1px;border-left-color: white;}";

int set_skill_all;
std::ofstream fout("result.txt");
twr* arr_t = {0};

double work_time = 0;
double break_time = 0;
bool res_is_get = 0;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ramka->setStyleSheet("QLabel {"
                             "border-style: solid;"
                             "border-width: 1px;"
                             "border-color: gray; "
                             "}");

    wG = new QCustomPlot();
    ui->gridLayout_2->addWidget(wG);
    wG->setBackground(QBrush(QColor("#EEEEEE")));
    wG->plotLayout()->insertRow(0);
    wG->plotLayout()->addElement(0, 0, new QCPTextElement(wG, "resource allocation among stuff", QFont("arial", 12, QFont::Bold)));



    wG2 = new QCustomPlot();
    ui->gridLayout_3->addWidget(wG2);
    wG2->setBackground(QBrush(QColor("#EEEEEE")));
    wG2->plotLayout()->insertRow(0);
    wG2->plotLayout()->addElement(0, 0, new QCPTextElement(wG2, "schedule", QFont("arial", 12, QFont::Bold)));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    wG2->xAxis->setTicker(timeTicker);

    close_menu();
}


MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_set_value_clicked(){
    set_skill_all = 0;
    MAX_POP =  ui->MAX_POP_3->text().toInt();
    MAX_STUFF = ui->MAX_STUFF_3->text().toInt();
    MAX_TASK = ui->MAX_TASK_3->text().toInt();
    MAX_TOTAL_TIME = ui->MAX_TOTAL_TIME_3->text().toInt();
    MAX_SOLUTION = ui->MAX_SOLUTION_3->text().toInt();
    EPS = ui->EPS_3->text().toFloat();
    pc = ui->pc_3->text().toFloat();
    pm = ui->pm_3->text().toFloat();
    count_skills = ui->count_skills_3->text().toInt();
    QString stylesheet = "::section{Background-color:rgb( 240,240,240);border-radius:14px;}";

    ui->tableWidget_3->setRowCount(1);
    ui->tableWidget_3->setColumnCount(3);
    ui->tableWidget_3->setRowCount(MAX_STUFF);

    QStringList lst;
    lst.append("№ Worker");
    lst.append("Call");
    lst.append("% Call");
    ui->tableWidget_3->setHorizontalHeaderLabels(lst);

    for(int i = 0; i < MAX_STUFF; i++){
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString::number(i+1));
        ui->tableWidget_3->setItem(i,0,item);
    }

    ui->tableWidget_3->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_3->verticalHeader()->setStyleSheet(stylesheet);
    ui->tableWidget_3->horizontalHeader()->setStyleSheet(stylesheet);

    //TASK
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(count_skills);
    ui->tableWidget->setRowCount(MAX_TASK);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    for(int i = 0; i < MAX_TASK; i++){
        for(int j=0;j<count_skills;j++){

            QWidget *checkBoxWidget = new QWidget();
            QCheckBox *checkBox = new QCheckBox();
            QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
            layoutCheckBox->addWidget(checkBox);
            layoutCheckBox->setAlignment(Qt::AlignCenter);
            layoutCheckBox->setContentsMargins(0,0,0,0);
            ui->tableWidget->setCellWidget(i,j, checkBoxWidget);
            if(j==0)
                checkBox->setCheckState(Qt::Checked);
            qApp->processEvents();
        }
    }
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setStyleSheet(stylesheet);
    ui->tableWidget->horizontalHeader()->setStyleSheet(stylesheet);

    //STUFF
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(count_skills);
    ui->tableWidget_2->setRowCount(MAX_STUFF);
    ui->tableWidget_2->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    for(int i = 0; i < MAX_STUFF; i++){
        for(int j=0;j<count_skills;j++){

            QWidget *checkBoxWidget = new QWidget();
            QCheckBox *checkBox = new QCheckBox();
            QHBoxLayout *layoutCheckBox = new QHBoxLayout(checkBoxWidget);
            layoutCheckBox->addWidget(checkBox);
            layoutCheckBox->setAlignment(Qt::AlignCenter);
            layoutCheckBox->setContentsMargins(0,0,0,0);
            ui->tableWidget_2->setCellWidget(i,j, checkBoxWidget);
            if(j==0){
                checkBox->setCheckState(Qt::Checked);
            }
            if(i==0){
                checkBox->setCheckState(Qt::Checked);
                checkBox->setEnabled(0);
            }

            qApp->processEvents();
        }
    }

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->verticalHeader()->setStyleSheet(stylesheet);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(stylesheet);

    ui->textBrowser->clear();
    ui->textBrowser->append("MAX_POP: " + QString::number(MAX_POP));
    ui->textBrowser->append("MAX_STUFF: " + QString::number(MAX_STUFF));
    ui->textBrowser->append("MAX_TASK: " + QString::number(MAX_TASK));
    ui->textBrowser->append("MAX_TOTAL_TIME: " + QString::number(MAX_TOTAL_TIME) + " s = " + QString::number(MAX_TOTAL_TIME/60) + " m = " + QString::number(MAX_TOTAL_TIME/60/60) + " h");
    ui->textBrowser->append("MAX_SOLUTION: " + QString::number(MAX_SOLUTION));
    ui->textBrowser->append("EPS: " + QString::number(EPS));
    ui->textBrowser->append("pc: " + QString::number(pc));
    ui->textBrowser->append("pm: " + QString::number(pm));
    ui->textBrowser->append("count_skills: " + QString::number(count_skills));
    QMessageBox::information(this, "Information","Completely");
}

void MainWindow::on_START_clicked(){
    if(set_skill_all < 2){
        QMessageBox::information(this, "Information","Skills not installed!");
        return;
    }

    emit on_go_to_setting_page_clicked();
    ui->solution->clear();
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0,1500 * MAX_SOLUTION);

    clock_t start = clock();
    srand((unsigned)time(NULL));

    population = new Chromosome[MAX_POP];
    Chromosome result;
    result = GA_Launch();
    Print_chromosome(result);
    clock_t stop = clock();
    double total_time_programm = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;

    QFile file("result.txt");
    QString data;
    if (!file.open(QIODevice::ReadOnly))
        ui->solution->append("Error: Не удается открыть файл");
    else{
        data = file.readAll();

        ui->solution->append(data);
        ui->solution->append("time to solve the last task: " + QString::number(total_time_programm) + "ms");
    }

    DrawGr();
}

void MainWindow::on_cleare_table_clicked(){
    ui->solution->clear();
    ui->progressBar->setValue(0);
}

Chromosome MainWindow::GA_Launch()
{
    bool test = 0;
    int c = 0;
    Chromosome result;
    Chromosome tmp;

    int progress = 0;
    ui->solution->append("finding the optimal solution");
    while (true && c < MAX_SOLUTION){
        ui->solution->append("[" + QString::number(c) + "] " + "processing...");
        if (!test){
            result = Solve(progress);
            test = check_result(result);
            if (c > 1)
                result = Compare_Chromosome(result, tmp);
            c++;
            tmp = result;
            progress = c * 1500;

        }
        else
            break;
        qApp->processEvents();
    }
    ui->progressBar->setValue(1500*MAX_SOLUTION);

    ui->solution->append("successful\n");
    return result;
}

Chromosome MainWindow::Solve(int progress){
    double *selection = new double[MAX_POP];
    int count_parents = MAX_POP;
    if (count_parents % 2 != 0)
        count_parents--;

    parents = new Chromosome[count_parents];
    Initial_population();
    int count_while = 0;
    fitness_function();

    Sort(population, MAX_POP);

    while (population[0].fitness > EPS && count_while <= 1500){
        ui->progressBar->setValue(progress + count_while);
        count_while++;
        Selection(selection, parents, count_parents);
        Crossing_mutation(parents, count_parents, count_while);
        qApp->processEvents();
    }

    return population[0];
}


void MainWindow::Crossing_mutation(Chromosome *parents, int count_parents, int count_while){
    Chromosome tmp;
    int i, j, rn;
    int max_pop = MAX_POP;
    int point_cross;
    Chromosome *population_and_children;
    population_and_children = new Chromosome[MAX_POP + count_parents / 2];

    for (int i = 0; i < MAX_POP; i++)
        population_and_children[i] = population[i];

    for (int k = 0; k < count_parents; k = k + 2){
        i = int(double(rand()) / RAND_MAX * (count_parents - 1));
        j = int(double(rand()) / RAND_MAX * (count_parents - 1));

        if (pc > double(rand()) / RAND_MAX){
            point_cross = rand() % MAX_TASK;
            rn = rand() % 2 + 1;

            for (int h = 0; h < MAX_TASK; h++){
                if (h < point_cross)
                    tmp.alleles[h].time = parents[i].alleles[h].time;
                else
                    tmp.alleles[h].time = parents[j].alleles[h].time;

                for (int z = 0; z < MAX_STUFF; z++)
                    tmp.alleles[h].number[z] = parents[i].alleles[h].number[z];
            }
            population_and_children[max_pop] = tmp;
            max_pop++;
        }

        qApp->processEvents();
    }

    double MAX_MUT = double(MAX_TOTAL_TIME) / double(MAX_TASK);
    double MIN_MUT = 0;
    float b = 1;
    double pm1 = pm * exp(-1.0 / count_while);
    for (int i = 1; i < max_pop; i++)
        for (int k = 0; k < MAX_TASK; k++){
            rn = rand() % 3;

            if (pm1 > double(rand()) / RAND_MAX)
                if (rn == 0)
                    population_and_children[i].alleles[k].time = population_and_children[i].alleles[k].time - Delta(population_and_children[i].alleles[k].time - MIN_MUT, count_while, b);
                else
                    population_and_children[i].alleles[k].time = population_and_children[i].alleles[k].time + Delta(MAX_MUT - population_and_children[i].alleles[k].time, count_while, b);

            qApp->processEvents();
        }

    New_population_set(population_and_children, max_pop);

    delete[]population_and_children;
}


void MainWindow::New_population_set(Chromosome *population_and_children, int new_max_pop){
    for (int i = 0; i < new_max_pop; i++){
        population_and_children[i].total_time = 0;
        for (int j = 0; j < MAX_TASK; j++)
            population_and_children[i].total_time += population_and_children[i].alleles[j].time;

        qApp->processEvents();
    }

    for (int i = 0; i < new_max_pop; i++){
        population_and_children[i].fitness = 0;
        population_and_children[i].fitness = abs(MAX_TOTAL_TIME - population_and_children[i].total_time);   
        qApp->processEvents();
    }

    Sort(population_and_children, new_max_pop);

    for (int i = 0; i < MAX_POP; i++)
        population[i] = population_and_children[i];
}


void MainWindow::Sort(Chromosome *A, int N) {
    int i = 0;
    int j = N - 1;
    Chromosome mid = A[N / 2];
    do {
        while (A[i].fitness + A[i].fitness_skill < mid.fitness + mid.fitness_skill)
            i++;
        while (A[j].fitness + A[i].fitness_skill > mid.fitness + mid.fitness_skill)
            j--;

        if (i <= j) {
            Chromosome tmp = A[i];
            A[i] = A[j];
            A[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);

    if (j > 0) {
        Sort(A, j + 1);
    }
    if (i < N) {
        Sort(&A[i], N - i);
    }
}

void MainWindow::Selection(double *selection, Chromosome *parents, int count_parents){
    double max, min;
    max = 0;
    min = 1;
    int k;
    double *r = new double[count_parents];
    double reverse_coef = 0;
    for (int i = 0; i < MAX_POP; i++)
        reverse_coef += 1 / (population[i].fitness + population[i].fitness_skill);

    for (int i = 0; i < MAX_POP; i++){
        selection[i] = 1 / (population[i].fitness + population[i].fitness_skill) / reverse_coef * 100;

        if (selection[i] > max)
            max = selection[i];
        if (selection[i] < min)
            min = selection[i];
        qApp->processEvents();
    }

    for (int i = 0; i < count_parents; i++){
        r[i] = double(rand()) / RAND_MAX * 100;

        if (r[i] >= max)
            parents[i] = population[0];
        else
            if (r[i] <= min)
                parents[i] = population[MAX_POP - 1];
            else{
                k = 0;
                while (r[i] < selection[k])
                    k++;
                if (k < MAX_POP)
                    parents[i] = population[k];
            }

        qApp->processEvents();
    }
    delete[] r;
}

void MainWindow::fitness_function(){
    for (int i = 0; i < MAX_POP; i++){
        population[i].fitness = 0;
        population[i].fitness = abs(MAX_TOTAL_TIME - population[i].total_time);

        qApp->processEvents();
    }
}

void MainWindow::Initial_population(){
    int NUM;
    for (int i = 0; i < MAX_POP; i++)
    {
        QMap <int, int> nt;
        for(int i=0; i<MAX_STUFF;i++){
            nt[i] = 0;
        }

        for (int j = 0; j < MAX_TASK; j++)
        {
            NUM = 0;
            population[i].alleles[j].time = double(rand()) / RAND_MAX;
            population[i].total_time += population[i].alleles[j].time;

            int index_stuff_skill = 0;
            int rng = task_s[j].rang;
            bool f = 0;
            bool f1 = 0;

            for(int i1 = rng - 1; i1 < count_skills;){
                for(int z = 0; z < MAX_STUFF;z++){
                    if(stuff_s[z].rang == rng){
                        f = 1;
                        for(int z1 = 0;z1<count_skills;z1++){
                            if(task_s[j].arr_skill[z1] == 1 && stuff_s[z].arr_skill[z1] == 0){
                                f = 0;
                            }
                        }

                        if(f == 1) {
                            index_stuff_skill = z;

                            for(int z2 = 0;z2<MAX_STUFF;z2++){
                                if(stuff_s[z2].rang == rng){
                                    f1 = 1;
                                    for(int z3 = 0;z3<count_skills;z3++){
                                        if(task_s[j].arr_skill[z3] == 1 && stuff_s[z2].arr_skill[z3] == 0){
                                            f1 = 0;
                                            break;
                                        }
                                    }
                                    if(nt[index_stuff_skill] > nt[z2] && f1){
                                        index_stuff_skill = z2;

                                    }
                                }
                                qApp->processEvents();
                            }
                        }
                    }
                    if(f == 1) break;
                }
                if(f == 1) break;
                rng += 1;
            }
            NUM = index_stuff_skill;
            nt[NUM]++;
            population[i].alleles[j].number[NUM] = 1;
            for (int k = 0; k < MAX_STUFF; k++){
                if (NUM != k){
                    population[i].alleles[j].number[k] = 0;
                }
            }

        }
    }
}

double MainWindow::Delta(double k, int count_while, float b){
    return k * (1.0 - pow(double(rand()) / RAND_MAX, ((1.0 - double(count_while / 1500)))*b));
}

void MainWindow::Print_chromosome(Chromosome tmp){
    double *average_time = new double[MAX_STUFF]();

    arr_t = new twr[MAX_TASK];

    int mas[MAX_STUFF];
    for (int k = 0; k < MAX_STUFF; k++){
        mas[k] = 0;
    }
    fout << "parameters:" << endl;
    fout << "1.MAX_POP: " << MAX_POP << endl;
    fout << "2.MAX_STUFF: " << MAX_STUFF << endl;
    fout << "3.MAX_TASK: " << MAX_TASK << endl;
    fout << "4.MAX_TOTAL_TIME: " << MAX_TOTAL_TIME << endl;
    fout << "5.MAX_SOLUTION: " << MAX_SOLUTION << endl;
    fout << "6.EPS: " << EPS << endl;
    fout << "7.pc: " << pc << endl;
    fout << "8.pm: " << pm << endl;
    fout << "=====================================================" << endl;
    for (int j = 0; j < MAX_TASK; j++)
    {
        arr_t[j].num_task = j + 1;

        int sk = 0;
        for(int z =0;z<count_skills;z++){
            sk*=10;
            sk+=task_s[j].arr_skill[z];
        }
        fout << "Task " << j + 1 << ": " <<sk<< endl;
        for (int k = 0; k < MAX_STUFF; k++)
            if (tmp.alleles[j].number[k] == 1)
            {

                arr_t[j].num_worker = k + 1;
                int st = 0;
                for(int z =0;z<count_skills;z++){
                    st*=10;
                    st+=stuff_s[k].arr_skill[z];
                }
                fout << "     Worker " << k + 1 <<": " <<st<<endl;
                average_time[k] += population[0].alleles[j].time;

                mas[k]++;

            }
        arr_t[j].resourse = tmp.alleles[j].time;
        fout << "     Time: " << tmp.alleles[j].time << endl;
        qApp->processEvents();
    }

    fout << endl << "Fitness: " << tmp.fitness << endl;
    fout << endl << "Total resources: " << tmp.total_time << endl;
    av_t = new double[MAX_STUFF]();
    for (int k = 0; k < MAX_STUFF; k++)
    {
        fout << "Worker " << k + 1 << ": " << average_time[k] << endl;
        av_t[k] = average_time[k];
        qApp->processEvents();
    }
    fout << endl << "=====================================================" << endl;

    for(int i=0;i<MAX_STUFF;i++){
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(QString::number(mas[i]));

        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setTextAlignment(Qt::AlignCenter);
        item1->setText(QString::number(double(mas[i])/double(MAX_TASK)*100) + " %");

        ui->tableWidget_3->setItem(i,1,item);
        ui->tableWidget_3->setItem(i,2,item1);
    }

    res_is_get = 1;
    delete[]average_time;
}

bool MainWindow::check_result(Chromosome tmp){
    double *average_time = new double[MAX_STUFF]();

    for (int j = 0; j < MAX_TASK; j++){
        for (int k = 0; k < MAX_STUFF; k++)
            if (tmp.alleles[j].number[k] == 1){
                average_time[k] += population[0].alleles[j].time;
            }
        qApp->processEvents();
    }

    int t = (MAX_TOTAL_TIME / MAX_STUFF );

    for (int k = 0; k < MAX_STUFF; k++){
        if (average_time[k] < t*0.8 || average_time[k] > t*1.2)
            return 0;
    }

    delete[]average_time;
    return 1;
}

Chromosome MainWindow::Compare_Chromosome(Chromosome tmp1, Chromosome tmp2){
    double *average_time1 = new double[MAX_STUFF]();
    double *average_time2 = new double[MAX_STUFF]();
    int t1 = 0;
    int t2 = 0;


    for (int j = 0; j < MAX_TASK; j++){
        for (int k = 0; k < MAX_STUFF; k++){
            if (tmp1.alleles[j].number[k] == 1)
                average_time1[k] += population[0].alleles[j].time;

            if (tmp2.alleles[j].number[k] == 1)
                average_time2[k] += population[0].alleles[j].time;
        }
        qApp->processEvents();
    }

    int t = (MAX_TOTAL_TIME / MAX_STUFF);
    for (int k = 0; k < MAX_STUFF; k++){
        if (average_time1[k] >= t * 0.8 && average_time1[k] <= t*1.2)
            t1++;
        if (average_time2[k] >= t * 0.8 && average_time2[k] <= t*1.2)
            t2++;
        qApp->processEvents();
    }

    delete[]average_time1;
    delete[]average_time2;

    if (t1 >= t2)
        return tmp1;
    else
        return tmp2;

}

void MainWindow::DrawGr(){
    wG->setInteraction(QCP::iRangeZoom,true);
    wG->setInteraction(QCP::iRangeDrag, true);
    wG->axisRect()->setRangeDrag(Qt::Horizontal);
    wG->axisRect()->setRangeZoom(Qt::Horizontal);
    wG->xAxis->setLabel("Stuff");
    wG->yAxis->setLabel("Resourse");

    QVector<double> x1(MAX_STUFF), y1(MAX_STUFF);
    for(int i = 0;i < MAX_STUFF; i++){
        x1[i] = i + 1;
        y1[i] = av_t[i];
        qApp->processEvents();
    }
    wG->xAxis2->setVisible(true);
    wG->xAxis2->setTickLabels(false);
    wG->yAxis2->setVisible(true);
    wG->yAxis2->setTickLabels(false);
    wG->addGraph(wG->xAxis, wG->yAxis);
    wG->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    wG->graph(0)->setPen(QPen("#0D7377"));
    wG->graph(0)->setData(x1,y1);

    for(int i = 0;i < MAX_STUFF; i++){
        QVector<double> x2(2),y2(2);
        x2[1] = x1[i];
        y2[1] = y1[i];
        x2[0] = x1[i];
        y2[0] = 0;
        wG->addGraph(wG->xAxis, wG->yAxis);
        const int cor = i + 1;
        wG->graph(cor)->setPen(QPen("#0D7377"));
        wG->graph(cor)->setData(x2,y2);
        qApp->processEvents();
        x2.clear();
        y2.clear();
    }

    emit on_zooming_clicked();
}

void MainWindow::DrawGr2(){
    wG2->clearGraphs();
    wG2->setInteraction(QCP::iRangeZoom,true);
    wG2->setInteraction(QCP::iRangeDrag, true);
    wG2->axisRect()->setRangeDrag(Qt::Horizontal);
    wG2->axisRect()->setRangeZoom(Qt::Horizontal);
    wG2->yAxis->setLabel("Stuff");

    int y_max = MAX_STUFF + 1;
    int s = 1;

    int k = 0;

    while(true){
        if(s*(MAX_TOTAL_TIME/MAX_TASK)>work_time){
            break;
        }
        else{s++;}
    }

    int count_w = 0;
    int ind_max = 0;
    float max;
    for(int i=0;i< MAX_STUFF;i++){
        if(max < av_t[i]){
            max = av_t[i];
            ind_max = i;
        }
        qApp->processEvents();
    }
    ind_max += 1;

    double new_work_time = s * (MAX_TOTAL_TIME / MAX_TASK);
    double new_break_time = break_time + new_work_time - work_time;
    double step = new_work_time + new_break_time;
    count_w = max / (work_time + break_time);

    wG2->xAxis->setRange(0, MAX_TOTAL_TIME/MAX_TASK + max);
    wG2->yAxis->setRange(0, MAX_STUFF + 1);

    QVector<double> x1(2), y1(2);
    for( int i=0;i<count_w+1;i++){
        x1[0] = (i+1)*step - new_break_time;
        y1[0] = 0;
        x1[1] = (i+1)*step - new_break_time;
        y1[1] = y_max;
        wG2->addGraph(wG2->xAxis, wG2->yAxis);
        wG2->graph(k)->setPen(QPen("#FF2E63"));
        wG2->graph(k)->setData(x1,y1);

        x1[0] = (i+1)*step;
        y1[0] = 0;
        x1[1] = (i+1)*step;
        y1[1] = y_max;
        wG2->addGraph(wG2->xAxis, wG2->yAxis);
        wG2->graph(k + 1)->setPen(QPen("#FF2E63"));
        wG2->graph(k + 1)->setData(x1,y1);
        k+=2;
    }


    double e;
    double point_x = 0;
    int bar = 1;

    for(int i=0;i<MAX_STUFF;i++){
        ui->solution->append("Worker " + QString::number(i+1) + ":");
        e = av_t[i] / (MAX_TOTAL_TIME/MAX_TASK) + 1;
        if(e>1){
            QVector<int> tmp;
            tmp = get_task(i+1);
            point_x = 0;
            bar = 1;
            QVector<double> x3(2), y3(2);
            QVector<double> x2(int(e)+1), y2(int(e)+1);
            for(int j = 0;j<int(e)+1;j++){


                y2[j] = i+1;
                x2[j] = point_x;


                if((float(x2[j] +  new_break_time) - float(bar*(step))) == 0){
                    x2[j] += new_break_time;
                    if(j < e - 1){
                        ui->solution->append("     Task " + QString::number(tmp.at(j)) + " :");
                        ui->solution->append("           START Task : " + QString::number(x2[j]) + " s = " + QString::number(x2[j]/60) + " m = " + QString::number(x2[j]/60/60) + " h");
                        ui->solution->append("           END Task : " + QString::number((x2[j] + MAX_TOTAL_TIME/MAX_TASK)) + " s = " + QString::number((x2[j] + MAX_TOTAL_TIME/MAX_TASK)/60) +
                                             " m = " + QString::number((x2[j] + MAX_TOTAL_TIME/MAX_TASK)/60/60) + " h");
                   }

                   bar++;
               }else{
                   if(j < e - 1){
                       ui->solution->append("     Task " + QString::number(tmp.at(j)) + " :");
                       ui->solution->append("           START Task : " + QString::number(x2[j]) + " s = " + QString::number(x2[j]/60) + " m = " + QString::number(x2[j]/60/60) + " h");
                       ui->solution->append("           END Task : " + QString::number((x2[j] + MAX_TOTAL_TIME/MAX_TASK)) + " s = " + QString::number((x2[j] + MAX_TOTAL_TIME/MAX_TASK)/60) +
                                            " m = " + QString::number((x2[j] + MAX_TOTAL_TIME/MAX_TASK)/60/60) + " h");

                   }
               }
               point_x = x2[j] + MAX_TOTAL_TIME/MAX_TASK;
           }

           wG2->addGraph(wG2->xAxis, wG2->yAxis);
           wG2->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
           wG2->graph(k)->setPen(QPen("#0D7377"));
           wG2->graph(k)->setData(x2,y2);


           k++;
           x2.clear();
           y2.clear();
           tmp.clear();
       }
   }
   qApp->processEvents();
   wG2->replot();
}
void MainWindow::on_Random_stuff_clicked(){
   if(count_skills == 1){
       QMessageBox::warning(this, "Information","The worker must have at least one skill");
       return;
   }
   int bar;
   for(int i = 1; i < MAX_STUFF; i++){
       for(int j = 1;j < count_skills;j++){
           QWidget *item = (ui->tableWidget_2->cellWidget(i,j));
           QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            bar = int(rand());
            if(bar % 2 == 0){
                checkB->setCheckState(Qt::Checked);
            }
            else{
                checkB->setCheckState(Qt::Unchecked);
            }
            qApp->processEvents();
        }
    }
}

void MainWindow::on_Random_task_clicked(){
    if(count_skills == 1){
        QMessageBox::warning(this, "Information","The worker must have at least one skill");
        return;
    }
    int bar;
    for(int i = 0; i < MAX_TASK; i++){
        for(int j = 1;j < count_skills;j++){
            QWidget *item = ( ui->tableWidget->cellWidget(i,j));
            QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            bar = int(rand());
            if(bar % 2 == 0){
                checkB->setCheckState(Qt::Checked);
            }
            else{
                checkB->setCheckState(Qt::Unchecked);
            }


            qApp->processEvents();
        }
    }
}

void MainWindow::on_set_skill_task_clicked(){
    set_skill_all++;
    task_s = new skill[MAX_TASK];
    for(int i = 0; i < MAX_TASK; i++){
        skill mass;
        mass.rang = 0;
        for(int j=0;j < count_skills;j++){


            QWidget *item = ( ui->tableWidget->cellWidget(i,j));
            QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            if(checkB->isChecked()){
                mass.arr_skill[j] = 1;
                mass.rang += 1;
            } else {
                mass.arr_skill[j] = 0;
            }
        }
        task_s[i] = mass;
    }

    QMessageBox::information(this, "Information","Completely");
}

void MainWindow::on_set_skill_stuff_clicked(){
    set_skill_all++;
    stuff_s = new skill[MAX_STUFF];
    for(int i = 0; i < MAX_STUFF; i++){
        skill mass;
        mass.rang = 0;
        for(int j=0;j < count_skills;j++){


            QWidget *item = ( ui->tableWidget_2->cellWidget(i,j));
            QCheckBox *checkB = qobject_cast <QCheckBox*> (item->layout()->itemAt(0)->widget());
            if(checkB->isChecked()){
                mass.arr_skill[j] = 1;
                mass.rang += 1;
            } else {
                mass.arr_skill[j] = 0;
            }
        }
        stuff_s[i] = mass;
    }

    QMessageBox::information(this, "Information","Completely");
}

void MainWindow::clear_all_btn(){
    QString btn = "QPushButton {background-color: rgb(33, 33, 33);border: none;color: #EEEEEE;}";
    QString btn_hover = "QPushButton:hover{background-color: #0D7377;border: none;color: #EEEEEE;}";
    QString btn_pressed = "QPushButton:pressed {      border: 2px solid #EEEEEE;border-radius: 2px;}";
    ui->go_to_setting_page->setStyleSheet(btn + btn_hover + btn_pressed);
    ui->go_to_skill_task_page->setStyleSheet(btn + btn_hover + btn_pressed);
    ui->go_to_skill_stuff_page->setStyleSheet(btn + btn_hover + btn_pressed);
    ui->go_to_gr_page->setStyleSheet(btn + btn_hover + btn_pressed);
    ui->go_to_phone_page->setStyleSheet(btn + btn_hover + btn_pressed);
    ui->go_to_gr_page->setStyleSheet(btn + btn_hover + btn_pressed);
    ui->go_to_gr_page_2->setStyleSheet(btn + btn_hover + btn_pressed);
}

void MainWindow::on_go_to_setting_page_clicked(){
    ui->label_13->setText("SETTING");
    if(ui->frame->width() == 130){
        close_menu();
        ui->go_to_setting_page->setStyleSheet("");
    }
    clear_all_btn();
    ui->go_to_setting_page->setStyleSheet(clk_btn);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_go_to_skill_task_page_clicked(){
    ui->label_13->setText("SKILLS TASK");
    if(ui->frame->width() == 130){
        close_menu();
    }
    clear_all_btn();
    ui->go_to_skill_task_page->setStyleSheet(clk_btn);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_go_to_skill_stuff_page_clicked(){
    ui->label_13->setText("SKILLS STUFF");
    if(ui->frame->width() == 130){
        close_menu();
    }
    clear_all_btn();
    ui->go_to_skill_stuff_page->setStyleSheet(clk_btn);
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_go_to_gr_page_clicked(){
    ui->label_13->setText("GRAPH 1");
    if(ui->frame->width() == 130){
        close_menu();
    }
    clear_all_btn();
    ui->go_to_gr_page->setStyleSheet(clk_btn);
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_MENU_clicked(){
    if(ui->frame->width() == 60){
        open_menu();
    }
    else {
        close_menu();
    }
}

void MainWindow::on_HOME_clicked(){
    ui->label_13->setText("");
    clear_all_btn();
    ui->stackedWidget->setCurrentIndex(0);
    close_menu();
}

void MainWindow::open_menu(){
    QPixmap pixmap("");
    QIcon ButtonIcon(pixmap);
    ui->frame->setFixedWidth(130);
    ui->MENU->setFixedWidth(130);
    ui->go_to_gr_page->setFixedWidth(130);
    ui->go_to_gr_page->setText("GRAPH 1");
    ui->go_to_gr_page->setIcon(ButtonIcon);
    ui->go_to_setting_page->setFixedWidth(130);
    ui->go_to_setting_page->setText("SETTING");
    ui->go_to_setting_page->setIcon(ButtonIcon);
    ui->go_to_skill_stuff_page->setFixedWidth(130);
    ui->go_to_skill_stuff_page->setText("SKILLS STUFF");
    ui->go_to_skill_stuff_page->setIcon(ButtonIcon);
    ui->go_to_skill_task_page->setFixedWidth(130);
    ui->go_to_skill_task_page->setText("SKILLS TASK");
    ui->go_to_skill_task_page->setIcon(ButtonIcon);
    ui->go_to_phone_page->setFixedWidth(130);
    ui->go_to_phone_page->setText("TABLE 1");
    ui->go_to_phone_page->setIcon(ButtonIcon);
    ui->START->setFixedWidth(130);
    ui->START->setText("START");
    ui->START->setIcon(ButtonIcon);
    ui->go_to_gr_page_2->setFixedWidth(130);
    ui->go_to_gr_page_2->setText("GRAPH 2");
    ui->go_to_gr_page_2->setIcon(ButtonIcon);
}


void MainWindow::close_menu(){
    ui->frame->setFixedWidth(60);

    ui->MENU->setFixedWidth(60);
    QPixmap pixmap(":/resourse/menu");
    QIcon ButtonIcon(pixmap);
    ui->MENU->setIcon(ButtonIcon);

    ui->go_to_setting_page->setFixedWidth(60);
    ui->go_to_setting_page->setText("");
    QPixmap pixmap2(":/resourse/setting");
    QIcon ButtonIcon2(pixmap2);
    ui->go_to_setting_page->setIcon(ButtonIcon2);

    ui->go_to_skill_task_page->setFixedWidth(60);
    ui->go_to_skill_task_page->setText("");
    QPixmap pixmap4(":/resourse/c_task");
    QIcon ButtonIcon4(pixmap4);
    ui->go_to_skill_task_page->setIcon(ButtonIcon4);

    ui->go_to_skill_stuff_page->setFixedWidth(60);
    ui->go_to_skill_stuff_page->setText("");
    QPixmap pixmap3(":/resourse/c_stuff");
    QIcon ButtonIcon3(pixmap3);
    ui->go_to_skill_stuff_page->setIcon(ButtonIcon3);

    ui->go_to_gr_page->setFixedWidth(60);
    ui->go_to_gr_page->setText("");
    QPixmap pixmap1(":/resourse/gr");
    QIcon ButtonIcon1(pixmap1);
    ui->go_to_gr_page->setIcon(ButtonIcon1);

    ui->START->setFixedWidth(60);
    ui->START->setText("");
    QPixmap pixmap5(":/resourse/start");
    QIcon ButtonIcon5(pixmap5);
    ui->START->setIcon(ButtonIcon5);

    ui->go_to_phone_page->setFixedWidth(60);
    ui->go_to_phone_page->setText("");
    QPixmap pixmap6(":/resourse/img/chat-2-64");
    QIcon ButtonIcon6(pixmap6);
    ui->go_to_phone_page->setIcon(ButtonIcon6);

    ui->go_to_gr_page_2->setFixedWidth(60);
    ui->go_to_gr_page_2->setText("");
    QPixmap pixmap7(":/resourse/calendar-9-64");
    QIcon ButtonIcon7(pixmap7);
    ui->go_to_gr_page_2->setIcon(ButtonIcon7);
}

void MainWindow::on_go_to_phone_page_clicked(){
    ui->label_13->setText("TABLE 1");
    if(ui->frame->width() == 130){
        close_menu();
    }
    clear_all_btn();
    ui->go_to_phone_page->setStyleSheet(clk_btn);
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_go_to_gr_page_2_clicked(){
    ui->label_13->setText("GRAPH 2");
    if(ui->frame->width() == 130){
        close_menu();
    }
    clear_all_btn();
    ui->go_to_gr_page_2->setStyleSheet(clk_btn);
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_zooming_clicked(){
    float max = 0;
    for(int i=0;i< MAX_STUFF;i++){
        if(max < av_t[i])
            max = av_t[i];
        qApp->processEvents();

    }
    const int coord_x = MAX_STUFF;
    const int coord_y = max + MAX_TOTAL_TIME/MAX_STUFF;
    wG->xAxis->setRange(0, coord_x + 1);
    wG->yAxis->setRange(0, coord_y);
    wG->replot();
}

void MainWindow::on_zooming_2_clicked(){
    float max;
    for(int i=0;i< MAX_STUFF;i++){
        if(max < av_t[i]){
            max = av_t[i];

        }
        qApp->processEvents();
    }
    wG2->xAxis->setRange(0, MAX_TOTAL_TIME/MAX_TASK + max);
    wG2->yAxis->setRange(0, MAX_STUFF + 1);
    wG2->replot();
}

void MainWindow::on_draw_gr2_clicked(){
    if(work_time==0){
        QMessageBox::information(this, "Information","Set parameters");
        return;
    }

    if(res_is_get==0){
        QMessageBox::information(this, "Information","Get results GA");
        return;
    }
    ui->solution->append("\n========================schedule========================");
    DrawGr2();
}

void MainWindow::on_set_par_clicked(){
    ui->param->clear();
    work_time =  ui->work_time->text().toFloat();
    break_time =  ui->break_time->text().toFloat();
    ui->param->append("work_time: " + QString::number(work_time) + " s = "  + QString::number(work_time/60) + " m = " + QString::number(work_time/60/60) + " h");
    ui->param->append("break_time: " + QString::number(break_time)  + " s = " +  QString::number(break_time/60) + " m = " + QString::number(break_time/60/60) + " h");
    int s = 1;


    while(true){
        if(s*(MAX_TOTAL_TIME/MAX_TASK)>work_time){
            break;
        }
        else{s++;}
    }

    double new_work_time = s * (MAX_TOTAL_TIME / MAX_TASK);
    double new_break_time = break_time + new_work_time - work_time;
    ui->param->append("*new_work_time: " + QString::number(new_work_time) + " s = "  + QString::number(new_work_time/60) + " m = " + QString::number(new_work_time/60/60) + " h");
    ui->param->append("*new_break_time: " + QString::number(new_break_time)  + " s = " +  QString::number(new_break_time/60) + " m = " + QString::number(new_break_time/60/60) + " h");

    QMessageBox::information(this, "Information","Completely");
}

QVector<int> MainWindow::get_task(int num_w){
    QVector<int> res;
    for(int i=0;i<MAX_TASK;i++){
        if(arr_t[i].num_worker == num_w){
            res.push_back(arr_t[i].num_task);
        }
    }
    return res;
}
