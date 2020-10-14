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

/// button initializing all calculations
void Magnetometer::on_calculate_pushButton_clicked()
{
    QVector<long long> input = get_ui_input();
    if(check_input(input))
    {
        QVector<float> xyzt_values = convert_units(get_xyzt_values(input[0], input[1]));
        float azimuth = calculate_azimuth(xyzt_values[0], xyzt_values[1]);
        update_ui_data(xyzt_values, azimuth);
    }
}

QVector<long long> Magnetometer::get_ui_input()
{
    QVector<long long> ui_input(2,0);

    ui_input[0] = ui->x28_input_lineEdit->text().toLongLong();
    ui_input[1] = ui->x29_input_lineEdit->text().toLongLong();

    return ui_input;
}

void Magnetometer::update_ui_data(QVector<float> xyzt_values, float azimuth)
{
    ui->x_lineEdit->setText(QString::number(xyzt_values[0]));
    ui->y_lineEdit->setText(QString::number(xyzt_values[1]));
    ui->z_lineEdit->setText(QString::number(xyzt_values[2]));
    ui->temp_lineEdit->setText(QString::number(xyzt_values[3]));
    ui->azimuth_lineEdit->setText(QString::number(azimuth));
}

bool Magnetometer::check_input(QVector<long long> input)
{
    if(input[0] < 4294967296 && input[1] < 4294967296 && input[0] >= 0 && input[1] >= 0)
        return true;
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Input out of range. Must be a positive integer, maximally 4294967295");
        msgBox.exec();
        return false;
    }
}

/// simple conversion - index 0 for LSB in all binary vectors
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

///valid  for positive or nagative binary numbers, index 0 for LSB
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


/// calculate XYZ magnetic field strength and IC temperature based on 0x28 and 0x29 registry values
/// returns cector of ints (0-Xaxis, 1-Yaxis, 2-Zaxis, 3-temperature)
QVector<int> Magnetometer::get_xyzt_values(long long x28_reg, long long x29_reg)
{
    QVector<int> xyzt_values(4, 0);

    QVector<int> x28_reg_bin = decimal_to_binary(x28_reg), x29_reg_bin = decimal_to_binary(x29_reg);
    QVector<QVector<int>> xyzt_bin = get_xyzt_bin(x28_reg_bin, x29_reg_bin);

    for (int i = 0; i < 4 ; i++)
    {
        xyzt_values[i] = binary_to_decimal(xyzt_bin[i]);
    }

    return xyzt_values;
}

/// translate binary 0x28 and 0x29 values to binary XYZ and temperature
QVector<QVector<int>> Magnetometer::get_xyzt_bin(QVector<int> x28_reg_bin, QVector<int> x29_reg_bin)
{
    QVector<int> bin(12,0);
    QVector<QVector<int>> xyzt_bin(4, bin);

    for(int i = 0; i < 4; i++)
    {
        xyzt_bin[2][i] = x29_reg_bin[i+8];
        xyzt_bin[1][i] = x29_reg_bin[i+12];
        xyzt_bin[0][i] = x29_reg_bin[i+16];
    }
    for(int i = 0; i < 8; i++)
    {
        xyzt_bin[2][i+4] = x28_reg_bin[i+8];
        xyzt_bin[1][i+4] = x28_reg_bin[i+16];
        xyzt_bin[0][i+4] = x28_reg_bin[i+24];
    }
    for(int i = 0; i < 6; i++)
    {
        xyzt_bin[3][i] = x29_reg_bin[i];
        xyzt_bin[3][i+6] = x28_reg_bin[i];
    }

    return xyzt_bin;
}

/// converts magnetic field strength to Gauss and temperature to Celcius
QVector<float> Magnetometer::convert_units(QVector<int> xyzt_from_reg)
{
    QVector<float> actual_xyzt_values(4, 0);

    for(int i = 0; i < 3; i++)
    {
        actual_xyzt_values[i] = (float)(xyzt_from_reg[i]) / 4;
    }
    actual_xyzt_values[3] = 302 * ((float)(xyzt_from_reg[3]) - 1708) / 4096;

    return actual_xyzt_values;
}


/// calculating azimuth - angle between device orientation and magnetic north
/// magnetometer results point at magneic north
/// phone points at x axis, y axis is to right, z axis is cofing from the screen
/// returns value in degrees
float Magnetometer::calculate_azimuth(float x, float y)
{
    // x > 0, y > 0, az =  360 - atan(y/x) * 180 / PI;
    // x > 0, y < 0, az =  -atan(y/x) * 180 / PI;
    // x < 0, y > 0, az =  180 - atan(y/x) * 180 / PI;
    // x > 0, y < 0, az =  180 + atan(y/x) * 180 / PI;

    if(y == 0 && x == 0)
        return -1;
    else if (y == 0 && x > 0)
        return 0;
    else if (x == 0)
        return 180 + 90 * sign(y);
    else if (x > 0)
        return 180 + 180 * sign(y) - atan(y/x) * 180 / PI;
    else if (x < 0)
        return 180 + sign(y) * atan(y/x) * 180 / PI;

    return -1;
}

int Magnetometer::sign(float x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}


