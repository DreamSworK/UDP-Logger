#include "settings.h"
#include "application.h"

#include <QInputDialog>
#include <QPushButton>
#include <QLabel>

ConfigLayout::ConfigLayout(QVariant *value, const QString &text) : QGridLayout(), value(value), text(text) {
    auto *button = new QPushButton(text);
    connect(button, &QPushButton::clicked, this, &ConfigLayout::setValue);

    valueLabel = new QLabel;
    valueLabel->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    valueLabel->setText(value->toString());

    this->addWidget(button, 0, 0);
    this->addWidget(valueLabel, 0, 1);
}

void ConfigLayout::setValue() const {
    if (value->type() == QVariant::Int) {
        const int intValue = QInputDialog::getInt(valueLabel, APPLICATION, text, value->toInt());
        valueLabel->setText(QString("%1").arg(intValue));
        *value = intValue;
    }
}


ConfigCheckBox::ConfigCheckBox(QVariant *value, const QString &text) : QCheckBox(text), value(value) {
    setChecked(value->toBool());
    connect(this, &ConfigCheckBox::stateChanged, this, &ConfigCheckBox::setValue);
}

void ConfigCheckBox::setValue(const int state) const {
    *value = static_cast<bool>(state);
}


SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent), mainWindow((QMainWindow*)parent) {
    setWindowTitle("Settings");
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    settings = new QSettings(ORGANIZATION, APPLICATION);
    loadSettings();

    auto *button = new QPushButton("Close");
    connect(button, &QPushButton::clicked, this, &SettingsWindow::close);

    auto *settingsLayout = new QVBoxLayout;
    settingsLayout->addWidget(new ConfigCheckBox(&values["autoStart"], QString("Auto start ") + APPLICATION));
    settingsLayout->addLayout(new ConfigLayout(&values["udpPort"], "UDP Port:"));
    settingsLayout->addWidget(button);
    setLayout(settingsLayout);
}

void SettingsWindow::loadSettings() {
    values["mainWindowPosition"] = settings->value("mainWindowPosition", QPoint(200, 200));
    values["mainWindowSize"] = settings->value("mainWindowSize", QSize(400, 400));
    values["lastDir"] = settings->value("lastDir", QApplication::applicationDirPath());
    values["autoStart"] = settings->value("autoStart", false);
    values["udpPort"] = settings->value("udpPort", 49152).toInt();
}

void SettingsWindow::saveSettings() {
    settings->setValue("mainWindowPosition", mainWindow->pos());
    settings->setValue("mainWindowSize", mainWindow->size());
    settings->setValue("lastDir", values["lastDir"].toString());
    settings->setValue("autoStart", values["autoStart"].toBool());
    settings->setValue("udpPort", values["udpPort"].toInt());
}

void SettingsWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    event->accept();
}