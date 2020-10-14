#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#define PI 3.14159265

#include <QMainWindow>
#include <math.h>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Magnetometer; }
QT_END_NAMESPACE

class Magnetometer : public QMainWindow
{
    Q_OBJECT

public:
    Magnetometer(QWidget *parent = nullptr);
    ~Magnetometer();

private slots:

    void on_calculate_pushButton_clicked();
    QVector<long long> get_ui_input();
    void update_ui_data(QVector<float> xyzt_values, float azimuth);
    bool check_input(QVector<long long> input);

    QVector<int> decimal_to_binary(long long decimal);
    int binary_to_decimal(QVector<int> binary);

    QVector<int> get_xyzt_values(long long x28_reg, long long x29_reg);
    QVector<QVector<int>> get_xyzt_bin(QVector<int> x28_reg_bin, QVector<int> x29_reg_bin);
    QVector<float> convert_units(QVector<int> xyzt_from_reg);

    float calculate_azimuth(float x, float y);
    int sign(float x);


private:
    Ui::Magnetometer *ui;
};
#endif // MAGNETOMETER_H
