#include "magnetometer.h"
#include "ui_magnetometer.h"

Magnetometer::Magnetometer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Magnetometer)
{
    ui->setupUi(this);
}

Magnetometer::~Magnetometer()
{
    delete ui;
}

void Magnetometer::on_calculate_pushButton_clicked()
{
    // max_value = 8589934592
    QVector<int> xyz_values = get_xyz_values(ui->x28_input_lineEdit->text().toLongLong(), ui->x29_input_lineEdit->text().toLongLong());
}

QVector<int> Magnetometer::get_xyz_values(long long x28_reg, long long x29_reg)
{
    QVector<int> xyz_values(3), x28_reg_bin = decimal_to_binary(x28_reg), x29_reg_bin = decimal_to_binary(x29_reg);

    return xyz_values;
}

QVector<int> Magnetometer::decimal_to_binary(long long decimal)
{
    QVector<int> binary(32,0);

    for(int i=0; decimal > 0; i++)
    {
        binary[i] = decimal % 2;
        decimal = decimal / 2;
    }

    return binary;
}
