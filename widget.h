#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QStack>
class Widget : public QWidget
{
    Q_OBJECT
private:
    QPushButton** bt;
    QPushButton* TurnButtom;
    QStack<QString> stk;
    int** MapOfColor;
    int Turn = 0;
    int Atack_I = -1;
    int Atack_J = -1;
    int Points = 0;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool RandForAtack(qreal);
    bool checkWin();
public slots:
    void buttomClicked();
    void turn();
};

#endif // WIDGET_H
