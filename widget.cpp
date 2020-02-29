#include "widget.h"
#include <QGridLayout>
#include <QStack>
#include <QTime>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    //Делаем рандом рандомным
    QTime temp(0, 0, 0);
    qsrand(temp.secsTo(QTime::currentTime()));
    //Выделение памяти под кнопки
    bt = new QPushButton* [4];
    for(int i(0); i < 4; i++)
    {
        bt[i] = new QPushButton[4];
    }
    //Выделение памяти под карту цветов
    MapOfColor = new int* [4];
    for(int i(0); i < 4; i++)
    {
        MapOfColor[i] = new int[4];
    }
    //Выделение памяти под цвета
    QPalette red("red");
    QPalette blue ("blue");
    //Кнопка конца хода
    TurnButtom = new QPushButton("Начать игру!");
    TurnButtom->setMinimumSize(200,50);
    TurnButtom->setMaximumSize(1200, 300);
    connect(TurnButtom, SIGNAL(clicked()), this, SLOT(turn()));
    //Основной слой
    QGridLayout* TopLay = new QGridLayout;
    //Распологаем кнопки и составляем карту цветов, распределяем очки
    for(int i(0); i < 4; i++)
        for(int j(0); j < 4; j++)
        {
            if(i == 0 && j == 0)
            {
                bt[i][j].setText("4");
                bt[i][j].setPalette(blue);
                MapOfColor[i][j] = -1;
                bt[i][j].setMinimumSize(50, 50);
                bt[i][j].setMaximumSize(300, 300);
                connect(&bt[i][j],SIGNAL(clicked()),this,SLOT(buttomClicked()));
                TopLay->addWidget(&bt[i][j], i, j);
            }
            else if(i == 3 && j == 3)
            {
                bt[i][j].setText("3");
                bt[i][j].setPalette(red);
                MapOfColor[i][j] = 1;
                bt[i][j].setMinimumSize(50, 50);
                bt[i][j].setMaximumSize(300, 300);
                connect(&bt[i][j],SIGNAL(clicked()),this,SLOT(buttomClicked()));
                TopLay->addWidget(&bt[i][j], i, j);
            }
            else
            {
                bt[i][j].setText("0");
                bt[i][j].setMinimumSize(50, 50);
                bt[i][j].setMaximumSize(300, 300);
                MapOfColor[i][j] = 0;
                connect(&bt[i][j],SIGNAL(clicked()),this,SLOT(buttomClicked()));
                TopLay->addWidget(&bt[i][j], i, j);
            }

        }
    TopLay->addWidget(TurnButtom, 5, 0, 1, 4);
    //Устанавливаем слой
    setLayout(TopLay);
}

//Диструктор
Widget::~Widget()
{
    //Удаляем кнопки
    delete TurnButtom;
    for(int i(0); i < 4; i++)
        delete[] bt[i];
    delete [] bt;
    //Удаляем карту цветов
    for (int i(0); i < 4; i++)
    {
        delete [] MapOfColor[i];
    }
    delete [] MapOfColor;
}

//Функция определяющая захвачена ли клетка при атаке
//Принимает разницу в очках
//Возвращает захвачено или нет
bool Widget::RandForAtack(qreal temp)
{
    //Если разница больше 2 захват 100%
    if(temp > 2)
        return true;
    //Если разница меньше -2 незахват 100%
    if(temp < -2)
        return false;
    //Другие вероятности
    switch ((int)temp)
    {
    case 0:         //Разницы нету шанс 50%
    {
        if(qrand()%2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case 1:         //Разница 1 шанс 75%
    {
        if(qrand()%3)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    case 2:
    {
        return true;
    }
    case -1:
    {
        if(qrand()%3)   //Разница -1 шанс 25%
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    case -2:
    {
        return false;
    }
    default:
    {
        return false;
    }
    }
}

//Функция проверки победы
//Если на поле только один из цветов игроков возвращает победу
bool Widget::checkWin()
{
    //Смотрим цвет в уголке
    int temp = MapOfColor[0][0];
    //Сравниваем с остальными цветами
    for(int i(0); i < 4; i++)
        for(int j(0); j < 4; j++)
        {
            //Если есть различия, то победы нету
            if(MapOfColor[i][j] == -temp)
            {
                return false;
            }
        }
    //Если нету, то победа
    return true;
}

//Слот принимающий сигналы с поля
void Widget::buttomClicked()
{
    //Если игра не началась - не реагируем
    if(Turn == 0)
    {
        return;
    }
    //Цвета для раскраски
    QPalette red("red");
    QPalette blue ("blue");
    //Считываем кнопку
    QString str = ((QPushButton*)sender())->text();
    //Определяем координаты отправителя сигнала
    int ii = 0;
    int jj = 0;
    for(int i(0); i < 4; i++)
        for(int j(0); j < 4; j++)
            if((QPushButton*)sender() == &bt[i][j])
            {
                //Запоминаем координаты
                ii = i;
                jj = j;
                //Выходим из цикла
                i = 4;
                break;
            }
    //Проверяем режим
    //Первый этап игры Красного игрока
    if(Turn == 1)
    {
        //Если в стеке пусто и в клетке более 1 очка
        if(stk.count() == 0 && ((QPushButton*)sender())->text() != "1")
        {
            //А цвет красный
            if(MapOfColor[ii][jj] == 1)
            {
                //Запоминаем Атакующего
                Atack_I = ii;
                Atack_J = jj;
                //Вствляем значение кнопки в стек
                stk.push(str);
                //Блокируем кнопки
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                            if(!((i == ii + 1 && jj == j) || (i == ii - 1 && jj == j) || (j == jj + 1 && ii == i) || (j == jj - 1 && ii == i)))
                            {
                                //Блокируем кнопки
                                bt[i][j].setEnabled(false);
                            }
                        }
            }
            else
            {
                //Если нажал не то
                TurnButtom->setText("Нажмите на свой цвет!(Красный)");
            }
        }
        //Если в стеке есть 1 элемент
        else if(stk.count() == 1)
        {
            //А цвет нейтральный или вражеский
            if(MapOfColor[ii][jj] == 0 || MapOfColor[ii][jj] == -1)
            {
                //Ложим в стек
                stk.push(str);
                //Разблокируем кнопки
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                                //Разблокируем кнопки
                                bt[i][j].setEnabled(true);
                        }
                //----------------Проводим Атаку------------------------
                //Извлекаем очки Атакующего и защищающегося из стека
                qreal Defend = stk.pop().toDouble();
                qreal Atack = stk.pop().toDouble();
                qreal temp = Atack;
                //Очищаем стек
                stk.clear();
                //Подсчитываем урон обоим сторонам
                Atack -= Defend;
                Defend -= temp;
                //Проверяем успешность захвата
                if(RandForAtack(Atack))
                {

                    if(Defend < 1)
                    {
                        Defend = 1;
                        Atack--;
                    }
                    //Если успешный, то проверяем чтобы минимум очков был раве 1
                    if(Atack < 1)
                        Atack = 1;
                    //Перекрашиваем кнопку
                    ((QPushButton*)sender())->setPalette(red);
                    MapOfColor[ii][jj] = 1;
                    str.setNum(Atack);
                    //Перераспределяем очки
                    ((QPushButton*)sender())->setText(str);
                    bt[Atack_I][Atack_J].setText("1");
                    //Обнуляем
                    Atack_I = -1;
                    Atack_J = -1;
                }
                else
                {
                    //Если неуспешный, то проверяем чтобы минимум очков был раве 1
                    if(Atack < 1)
                        Atack = 1;
                    //Перераспределяем очки
                    str.setNum(Atack);
                    bt[Atack_I][Atack_J].setText(str);
                    if(Defend < 1)
                        Defend = 1;
                    str.setNum(Defend);
                    ((QPushButton*)sender())->setText(str);
                    //Обнуляем
                    Atack_I = -1;
                    Atack_J = -1;
                }
                //------------------------------------------------------
                //Проверяем победили ли
                if(checkWin())
                {
                    TurnButtom->setText("Красные победили!");
                    Turn = 3;
                }
            }
            else if(MapOfColor[ii][jj] == 1 && (&bt[Atack_I][Atack_J] != ((QPushButton*)sender())))
            {
                int temp = 0;
                temp = ((QPushButton*)sender())->text().toInt();
                temp += bt[Atack_I][Atack_J].text().toInt();
                temp--;
                str.setNum(temp);
                ((QPushButton*)sender())->setText(str);
                bt[Atack_I][Atack_J].setText("1");
                //Обнуляем
                Atack_I = -1;
                Atack_J = -1;
                stk.clear();
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                                //Разблокируем кнопки
                                bt[i][j].setEnabled(true);
                        }
            }
            else
            {
                //Если нажали не туда
                stk.clear();
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                                //Разблокируем кнопки
                                bt[i][j].setEnabled(true);
                        }
                //Если нажал не то
                TurnButtom->setText("Нажмите на отличный от вашего цвет!");
            }
        }
    }
    //Второй этап игры Красного игрока
    if(Turn == 2)
    {
        //Пока есть очки и правильный цвет
        if(Points > 0 && MapOfColor[ii][jj] == 1)
        {
            //Прибавляем очки
            qreal temp = str.toDouble() + 1;
            str.setNum(temp);
            ((QPushButton*)sender())->setText(str);
            QString temp_string;
            temp_string = "Распределите очки, осталось: ";
            Points--;
            str.setNum(Points);
            temp_string += str;
            TurnButtom->setText(temp_string);
        }
    }
    //Первый этап игры Синего игрока
    if(Turn == -1)
    {
        //Если в стеке пусто
        if(stk.count() == 0 && ((QPushButton*)sender())->text() != "1")
        {
            //А цвет синий
            if(MapOfColor[ii][jj] == -1)
            {
                Atack_I = ii;
                Atack_J = jj;
                //Вствляем значение кнопки в стек
                stk.push(str);
                //Блокируем кнопки
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                            if(!((i == ii + 1 && jj == j) || (i == ii - 1 && jj == j) || (j == jj + 1 && ii == i) || (j == jj - 1 && ii == i)))
                            {
                                //Блокируем кнопки
                                bt[i][j].setEnabled(false);
                            }
                        }
            }
            else
            {
                //Если нажал не то
                TurnButtom->setText("Нажмите на свой цвет!(Синий)");
            }
        }
        //Если в стеке есть 1 элемент
        else if(stk.count() == 1)
        {
            //А цвет нейтральный или вражеский
            if(MapOfColor[ii][jj] == 0 || MapOfColor[ii][jj] == 1)
            {
                //Ложим в стек
                stk.push(str);
                //Разблокируем кнопки
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                                //Разблокируем кнопки
                                bt[i][j].setEnabled(true);
                        }
                //----------------Проводим Атаку------------------------
                //Извлекаем очки Атакующего и защищающегося из стека
                qreal Defend = stk.pop().toDouble();
                qreal Atack = stk.pop().toDouble();
                qreal temp = Atack;
                stk.clear();
                //Подсчитываем урон обоим сторонам
                Atack -= Defend;
                Defend -= temp;
                //Проверяем успешность захвата (Аналогично см.выше)
                if(RandForAtack(Atack))
                {
                    if(Defend < 1)
                    {
                        Defend = 1;
                        Atack--;
                    }
                    if(Atack < 1)
                        Atack = 1;
                    ((QPushButton*)sender())->setPalette(blue);
                    str.setNum(Atack);
                    ((QPushButton*)sender())->setText(str);
                    bt[Atack_I][Atack_J].setText("1");
                    MapOfColor[ii][jj] = -1;
                    Atack_I = -1;
                    Atack_J = -1;
                }
                else
                {
                    if(Atack < 1)
                        Atack = 1;
                    str.setNum(Atack);
                    bt[Atack_I][Atack_J].setText(str);
                    if(Defend < 1)
                        Defend = 1;
                    str.setNum(Defend);
                    ((QPushButton*)sender())->setText(str);
                    Atack_I = -1;
                    Atack_J = -1;
                }
                //------------------------------------------------------
                if(checkWin())
                {
                    TurnButtom->setText("Синие победили!");
                    Turn = -3;
                }
            }
            else if(MapOfColor[ii][jj] == -1 && (&bt[Atack_I][Atack_J] != ((QPushButton*)sender())))
            {
                int temp = 0;
                temp = ((QPushButton*)sender())->text().toInt();
                temp += bt[Atack_I][Atack_J].text().toInt();
                temp--;
                str.setNum(temp);
                ((QPushButton*)sender())->setText(str);
                bt[Atack_I][Atack_J].setText("1");
                //Обнуляем
                Atack_I = -1;
                Atack_J = -1;
                stk.clear();
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                                //Разблокируем кнопки
                                bt[i][j].setEnabled(true);
                        }
            }
            else
            {
                stk.clear();
                for(int i(0); i < 4; i++)
                        for(int j(0); j < 4; j++)
                        {
                                //Разблокируем кнопки
                                bt[i][j].setEnabled(true);
                        }
                //Если нажал не то
                TurnButtom->setText("Нажмите на отличный от вашего цвет!");
            }
        }
    }
    //Второй этап игры Синего игрока
    if(Turn == -2)
    {
        if(Points > 0 && MapOfColor[ii][jj] == -1)
        {
            qreal temp = str.toDouble() + 1;
            str.setNum(temp);
            ((QPushButton*)sender())->setText(str);
            QString temp_string;
            temp_string = "Распределите очки, осталось: ";
            Points--;
            str.setNum(Points);
            temp_string += str;
            TurnButtom->setText(temp_string);
        }
    }
}

//Слот меняющий состояние игры (Определяет ход и этап хода игрока)
void Widget::turn()
{
    //Временные переменные
    QString str_temp;
    qreal temp = 0;
    //Игра не началась
    if(Turn == 0)
    {
        //Очищаем стек
        stk.clear();
        //Отдаем ход 1 игроку
        Turn += 1;
        TurnButtom->setText("Ход Красного игрока");
    }
    //Если первый игрок совершил атаку
    else if(Turn == 1)
    {
        //Считаем очки
        for(int i(0); i < 4; i++)
            for(int j(0); j < 4; j++)
            {
                if(MapOfColor[i][j] == 1)
                {
                    Points++;
                }
            }
        //Даем 1 игроку распределить очки
        Turn += 1;
        TurnButtom->setText("Конец хода\n Распределите очки");
    }
    //Первый игрок распределил очки
    else if(Turn == 2)
    {
        //Разблокируем кнопки (На всякий случай)
        for(int i(0); i < 4; i++)
                for(int j(0); j < 4; j++)
                {
                        //Разблокируем кнопки
                        bt[i][j].setEnabled(true);
                }
        //Если ещё остались очки
        if(Points > 0)
        {
            for(int i(3); i > -1; i--)
                for(int j(3); j > -1; j--)
                {
                    if(MapOfColor[i][j] == 1)
                    {
                        if(Points > 0)
                        {
                            //Распределяем по порядку
                            str_temp = bt[i][j].text();
                            temp = str_temp.toDouble();
                            temp++;
                            str_temp.setNum(temp);
                            bt[i][j].setText(str_temp);
                            Points--;
                        }
                    }
                }
        }
        Points = 0;
        stk.clear();
        Turn = -1;
        TurnButtom->setText("Ход Синего игрока");
    }
    //Аналогично для синего
    else if(Turn == -1)
    {
        for(int i(0); i < 4; i++)
            for(int j(0); j < 4; j++)
            {
                if(MapOfColor[i][j] == -1)
                {
                    Points++;
                }
            }
         Turn -= 1;
         TurnButtom->setText("Конец хода\n Распределите очки");
    }
    else if(Turn == -2)
    {
        //Разблокируем кнопки
        for(int i(0); i < 4; i++)
                for(int j(0); j < 4; j++)
                {
                        //Разблокируем кнопки
                        bt[i][j].setEnabled(true);
                }
        if(Points > 0)
        {
            for(int i(3); i > -1; i--)
                for(int j(3); j > -1; j--)
                {
                    if(MapOfColor[i][j] == -1)
                    {
                        if(Points > 0)
                        {
                            str_temp = bt[i][j].text();
                            temp = str_temp.toDouble();
                            temp++;
                            str_temp.setNum(temp);
                            bt[i][j].setText(str_temp);
                            Points--;
                        }
                    }
                }
        }
        Points = 0;
        stk.clear();
        Turn = 1;
        TurnButtom->setText("Ход Красного игрока");
    }
}
