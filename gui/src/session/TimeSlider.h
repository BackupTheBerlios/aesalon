#ifndef AESALON_GUI_SESSION_TIME_SLIDER_H
#define AESALON_GUI_SESSION_TIME_SLIDER_H

#include <QDoubleSpinBox>

class TimeSlider : public QSpinBox { Q_OBJECT
public:
    TimeSlider(QWidget *parent = NULL);
    virtual ~TimeSlider();
    
    virtual QString textFromValue(int value) const;
    virtual int valueFromText(const QString &text) const;
    virtual QValidator::State validate(QString &input, int &pos) const;
    virtual void stepBy(int steps);
    virtual void fixup(QString &input) const;
};

#endif
