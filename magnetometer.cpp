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
    QVector<int> xyz_values = get_xyzt_values(ui->x28_input_lineEdit->text().toLongLong(), ui->x29_input_lineEdit->text().toLongLong());
    ui->x_lineEdit->setText(QString::number(xyz_values[0]));
    ui->y_lineEdit->setText(QString::number(xyz_values[1]));
    ui->z_lineEdit->setText(QString::number(xyz_values[2]));
    ui->temp_lineEdit->setText(QString::number(xyz_values[3]));
}

QVector<int> Magnetometer::get_xyzt_values(long long x28_reg, long long x29_reg)
{
    // index 0 for LSB in all binary vectors
    QVector<int> xyz_values(4, 0);
    if(x28_reg < 8589934592 && x29_reg < 8589934592 && x28_reg >= 0 && x29_reg >= 0)
    {
        QVector<int> x28_reg_bin = decimal_to_binary(x28_reg), x29_reg_bin = decimal_to_binary(x29_reg);
        QVector<QVector<int>> xyz_bin = get_xyz_bin(x28_reg_bin, x29_reg_bin);

        for (int i = 0; i < 4 ; i++)
        {
            xyz_values[i] = binary_to_decimal(xyz_bin[i]);
        }
    }

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

///valid  for positive or nagative binary numbers
int Magnetometer::binary_to_decimal(QVector<int> binary)
{
    int decimal = 0;

    if(binary[binary.length()-1] == 0)
    {
        for (int i = 0; i < binary.length() ; i++)
        {
            decimal += binary[i] * pow(2,i);
        }
    }
    else
    {
        decimal--;
        for (int i = 0; i < binary.length()-1 ; i++)
        {
            decimal += (binary[i]-1) * pow(2,i);
        }
    }

    return decimal;
}

QVector<QVector<int>> Magnetometer::get_xyz_bin(QVector<int> x28_reg_bin, QVector<int> x29_reg_bin)
{
    QVector<int> bin(12,0);
    QVector<QVector<int>> xyz_bin(4, bin);

    for(int i = 0; i < 4; i++)
    {
        xyz_bin[2][i] = x29_reg_bin[i+8];
        xyz_bin[1][i] = x29_reg_bin[i+12];
        xyz_bin[0][i] = x29_reg_bin[i+16];
    }
    for(int i = 0; i < 8; i++)
    {
        xyz_bin[2][i+4] = x28_reg_bin[i+8];
        xyz_bin[1][i+4] = x28_reg_bin[i+16];
        xyz_bin[0][i+4] = x28_reg_bin[i+24];
    }
    for(int i = 0; i < 6; i++)
    {
        xyz_bin[3][i] = x29_reg_bin[i];
        xyz_bin[3][i+6] = x28_reg_bin[i];
    }

    return xyz_bin;
}


