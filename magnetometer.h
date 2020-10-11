#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include <QMainWindow>
#include <QVector2D>
#include <math.h>

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
    QVector<int> get_xyzt_values(long long x28_reg, long long x29_reg);
    QVector<int> decimal_to_binary(long long decimal);
    int binary_to_decimal(QVector<int> binary);
    QVector<QVector<int>> get_xyz_bin(QVector<int> x28_reg_bin, QVector<int> x29_reg_bin);

private:
    Ui::Magnetometer *ui;
};
#endif // MAGNETOMETER_H
