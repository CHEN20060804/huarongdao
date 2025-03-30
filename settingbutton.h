#ifndef SETTINGBUTTON_H
#define SETTINGBUTTON_H

#include "mainbutton.h"

class SettingButton : public MainButton
{
    Q_OBJECT

public:
    explicit SettingButton(QWidget *parent = nullptr);
    ~SettingButton();

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SETTINGBUTTON_H
