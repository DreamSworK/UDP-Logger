#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include <QtGui>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>
#include <QMainWindow>
#include <QString>
#include <QMap>

QT_BEGIN_NAMESPACE
class QLabel;
class QCheckBox;
class QPushButton;
class QInputDialog;
QT_END_NAMESPACE


class ConfigLayout : public QGridLayout
{
    Q_OBJECT
public:
    ConfigLayout(QVariant *value, const QString &text);
private slots:
    void setValue() const;
private:
    QString text;
    QLabel *valueLabel;
    QVariant *value;
};


class ConfigCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    ConfigCheckBox(QVariant *value, const QString &text);
private slots:
    void setValue(int state) const;
private:
    QVariant *value;
};


class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    SettingsWindow(QWidget *parent);
    QMap<QString, QVariant> values;
    void loadSettings();
    void saveSettings();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QSettings *settings;
    QMainWindow *mainWindow;
};

#endif