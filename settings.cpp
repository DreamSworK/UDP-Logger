#include "settings.h"
#include "application.h"

ConfigLayout::ConfigLayout(QVariant *value, const QString &text) : QGridLayout(), value(value), text(text) {
    QPushButton *setButton = new QPushButton(text);
    connect(setButton, SIGNAL(clicked()), this, SLOT(setValue()));

    valueLabel = new QLabel;
    valueLabel->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    valueLabel->setText(value->toString());

    this->addWidget(setButton, 0, 0);
    this->addWidget(valueLabel, 0, 1);
}

void ConfigLayout::setValue() {
    if (value->type() == QVariant::Int) {
        int intValue = QInputDialog::getInt(valueLabel, APP, text, value->toInt());
        valueLabel->setText(QString("%1").arg(intValue));
        *value = intValue;
    }
}


ConfigCheckBox::ConfigCheckBox(QVariant *value, const QString &text) : QCheckBox(text), value(value) {
    setChecked(value->toBool());
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(setValue(int)));
}

void ConfigCheckBox::setValue(int state) {
    *value = (bool) state;
}


SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent), mainWindow((QMainWindow*)parent) {
    setWindowTitle("Settings");
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    settings = new QSettings(ORG, APP);
    loadSettings();

    QPushButton *closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *settingsLayout = new QVBoxLayout;
    settingsLayout->addWidget(new ConfigCheckBox(&values["autoStart"], QString("Auto start ") + APP));
    settingsLayout->addLayout(new ConfigLayout(&values["udpPort"], "UDP Port:"));
    settingsLayout->addWidget(closeButton);
    setLayout(settingsLayout);
}

void SettingsWindow::loadSettings() {
    values["mainWindowPosition"] = settings->value("mainWindowPosition", QPoint(200, 200));
    values["mainWindowSize"] = settings->value("mainWindowSize", QSize(400, 400));
    values["lastDir"] = settings->value("lastDir", QApplication::applicationDirPath());
    values["autoStart"] = settings->value("autoStart", false);
    values["udpPort"] = settings->value("udpPort", 49152);
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