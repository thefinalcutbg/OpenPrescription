#pragma once

#include <QAbstractButton>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>

#include "Model/Patient.h"

class TileButton : public QAbstractButton
{
    void paintEvent(QPaintEvent* e) override;
    bool eventFilter(QObject* obj, QEvent* e) override;
    

protected:

    bool hover{ false };
    bool clicked{ false };
    bool m_reveresed{ false };
    QFont header;
    QFont info;
    QFont infoLabel;

    QString elide(const QString& text, int length);
    virtual void paintInfo(QPainter* painter) = 0;

public:
    TileButton(QWidget* parent = 0);
    void reverse() { m_reveresed = !m_reveresed; };
};

#include "View/uiComponents/IconButton.h"

class PatientTile : public TileButton
{

public:
    PatientTile(QWidget* parent = 0);
    void setData(const Patient& patient, int age);

private:

    QString idLabel;
    QString name;
    QString type;
    QString id;

    QString birthDate;
    QString age;
    QString address;
    QString phone;

    void paintInfo(QPainter* painter);
};
