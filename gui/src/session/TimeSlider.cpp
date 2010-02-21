#include <cmath>
#include <QTextStream>
#include <QDebug>
#include "TimeSlider.h"
#include "TimeSlider.moc"

TimeSlider::TimeSlider(QWidget* parent) : QSpinBox(parent) {
    setMinimum(0.0);
    setMaximum(6000000.0);
}

TimeSlider::~TimeSlider() {

}

QString TimeSlider::textFromValue(int value) const {
    int minutes = int(value/1000) / 60;
    int seconds = int(value/1000) % 60;
    int milliseconds = value % 1000;
    QString text;
    text = text.sprintf("%02i:%02i.%03i", minutes, seconds, milliseconds);
    return text;
}

int TimeSlider::valueFromText(const QString& text) const {
    int minutes = 0;
    int seconds = 0, milliseconds = 0;
    char sep;
    QString string = text;
    QTextStream stream(&string);
    stream >> minutes;
    stream >> sep;
    stream >> sep;
    seconds += (sep - '0') * 10;
    stream >> sep;
    if(sep != '.') {
        seconds += (sep - '0');
        stream >> sep;
    }
    stream >> milliseconds;
    return ((minutes * 60) + seconds)*1000 + milliseconds;
}

QValidator::State TimeSlider::validate(QString& input, int& pos) const {
    QByteArray bytes = input.toAscii();
    int position = 0;
    bool read = false;
    if(input.indexOf(':') != -1) {
        read = true;
        for(; position < input.indexOf(':'); position ++) {
            if(!isdigit(bytes[position])) return QValidator::Invalid;
        }
        position ++;
    }
    if(input.indexOf('.') != -1) {
        read = true;
        int i = 0;
        for(; position < input.indexOf('.'); position ++) {
            if(!isdigit(bytes[position])) return QValidator::Invalid;
            i++;
        }
        if(i > 2) return QValidator::Invalid;
        position ++;
    }
    if(position < input.size()) {
        for(; position < input.size(); position ++) {
            if(!isdigit(bytes[position])) return QValidator::Invalid;
            read = true;
        }
    }
    if(read) {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}

void TimeSlider::stepBy(int steps) {
    setValue(value() + (steps*100));
}

void TimeSlider::fixup(QString& input) const {
    QTextStream stream(&input);
    double value = -0.1;
    stream >> value;
    input.setNum(int(value));
    input += ':';
    qDebug() << value;
    input += QString().setNum(fmod(value, 1.0) * 60);
}
