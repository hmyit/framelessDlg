#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include "NcFramelessHelper.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
};
/*---------------------------------*/

class State_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit State_Button(QWidget *parent = 0);
    ~State_Button();

public:
    void load_pximap(const QString& pic_name, int state_count=4);
    void set_pixmap(const QPixmap& pixmap, int state_count=4);

    void load_background(const QString& pic_name, int state_count=4);
    void set_background(const QPixmap& pixmap, int state_count=4);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

private:
    void paint_pixmap();

private:
    //枚举按钮的几种状态
    enum ButtonStatus {NORMAL, HOVER, PRESSED, DISABLED, CHECKED};
    //pximap_位图类型, 仅能选择其一. 如果两者都需要，请选择BACKGROUND + QPushButton::setIcon
    enum PixmapType   {FOREGROUND, BACKGROUND};

    QPixmap         pixmap_;        //图片
    PixmapType      pixmap_type_;
    int             state_count_;   //图片有几种状态(几张子图)
    ButtonStatus    status_;        //当前状态
    int             width_;         //按钮宽度
    int             height_;        //按钮高度
    bool            mouse_pressed_; //鼠标左键是否按下
};

/*---------------------------------*/
//无边框界面标题栏类
class Title_Bar: public QWidget
{
    Q_OBJECT
public:
    Title_Bar(QWidget *parent = NULL);
    ~Title_Bar();

    QString get_text(){return title->text();}
    void set_text(const QString& text){title->setText(text);}
    bool is_window_maximized() const {return is_maxed;}
protected:
    void init();
private slots:
    void show_small();
    void show_max_restore();
    void close_window();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QHBoxLayout*    titleLayout;
    QLabel*         icon;
    QLabel*         title;
    State_Button*   menu_button;
    State_Button*   min_button;
    State_Button*   max_button;
    State_Button*   close_button;
    QRect           normal_rect;
    QPoint          click_pos;      //
    bool            left_pressed;   //left mouse pressed
    bool            is_maxed;       //window Maximized
};
/*---------------------------------

class Main_Title_Bar: public Title_Bar
{
    Q_OBJECT
public:
    enum Show_Style_Ex{
        SHOW_SKIN            = 0x40,
        SHOW_MENU            = 0x80,
        MAIN_FORM_STYLE      = Title_Bar::FORM_STYLE|SHOW_MENU,
        MAIN_FORM_SKIN_STYLE = MAIN_FORM_STYLE | SHOW_SKIN
    };

public:
    Main_Title_Bar(QWidget* parent, Skin *skin = NULL);
    ~Main_Title_Bar();

signals:
    //on skin button clicked
    void skin_clicked();
    // on menu button clicked
    void menu_clicked();

public:
    //overload
    virtual void set_show_style(int style);
    virtual void load_skin(Skin* skin=NULL);

private:
    void init();
    void load_skin_internal();

protected:
    State_Button* skin_button_;
    State_Button* menu_button_;
};*/
/*----------------------------------*/




/*----------------------------------*/





/*----------------------------------*/




class frameLessDialog: public QDialog
{
    Q_OBJECT

public:
    frameLessDialog(QWidget *parent = 0);
    ~frameLessDialog();
private:
    NcFramelessHelper* fh;
};

#endif // DIALOG_H
