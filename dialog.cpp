#include "dialog.h"
#include <QStyle>
#include <QPainter>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QLabel>
#include <QDesktopWidget>
#include <QFile>
#include <QTextStream>

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QTableWidget>


#include <QtSvg>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
}

Dialog::~Dialog()
{

}

/*---------------------------------*/

State_Button::State_Button(QWidget *parent)
    :QPushButton(parent)
    ,status_(NORMAL)
    ,mouse_pressed_(false)
{
    setStyleSheet("QPushButton{background:transparent;}");
}

State_Button::~State_Button()
{
}

void State_Button::load_pximap(const QString& pic_name, int state_count)
{
    pixmap_type_ = FOREGROUND;
    pixmap_.load(pic_name);
    state_count_ = state_count;
    width_       = pixmap_.width() / state_count;
    height_      = pixmap_.height();
    setFixedSize(width_, height_);
}

void State_Button::set_pixmap( const QPixmap& pixmap, int state_count )
{
    pixmap_type_ = FOREGROUND;
    pixmap_      = pixmap;
    state_count_ = state_count;
    width_       = pixmap_.width() / state_count;
    height_      = pixmap_.height();
    setFixedSize(width_, height_);
}

void State_Button::load_background( const QString& pic_name, int state_count/*=4*/ )
{
    load_pximap(pic_name, state_count);
    pixmap_type_ = BACKGROUND;
}

void State_Button::set_background( const QPixmap& pixmap, int state_count/*=4*/ )
{
    set_pixmap(pixmap, state_count);
    pixmap_type_ = BACKGROUND;
}

void State_Button::enterEvent(QEvent *)
{
    status_ = HOVER;
    update();
}

void State_Button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouse_pressed_ = true;
        status_ = PRESSED;
        update();
    }

    QPushButton::mousePressEvent(event);
}

void State_Button::mouseReleaseEvent(QMouseEvent *event)
{
    if (mouse_pressed_)
    {
        mouse_pressed_ = false;

        if (this->rect().contains(event->pos()))
        {
            //if (isCheckable())
            //{
            //    setChecked(!isChecked());
            //}
            //emit clicked();

            status_ = HOVER;
        }else{
            status_ = NORMAL;
        }

        update();
    }

    QPushButton::mouseReleaseEvent(event);
}

void State_Button::leaveEvent(QEvent *)
{
    status_ = NORMAL;
    update();
}

void State_Button::paintEvent(QPaintEvent *e)
{
    if (pixmap_type_ == BACKGROUND)
    {
        paint_pixmap();
    }

    QPushButton::paintEvent(e);

    if (pixmap_type_ == FOREGROUND)
    {
        paint_pixmap();
    }
}

void State_Button::paint_pixmap()
{
    QPainter painter(this);

    //根据状态显示图片
    ButtonStatus status = status_;
    if (!isEnabled())
    {
        status = DISABLED;
    }
    else if (isChecked())
    {
        status = CHECKED;
        //没有checked图片，用hover代替
        if (status >= state_count_)
        {
            status = HOVER;
        }
    }

    if (status >= state_count_)
    {
        status = NORMAL;
    }

    painter.drawPixmap(rect(), pixmap_.copy(width_ * status, 0, width_, height_));
}
/*---------------------------------*/
Title_Bar::Title_Bar( QWidget* parent )
    :QWidget(parent)
    ,left_pressed(false)
    ,is_maxed(false)
{
    titleLayout     = new QHBoxLayout(this);

    icon            = new QLabel(this);
    title           = new QLabel(this);

    menu_button     = new State_Button(this);
    min_button      = new State_Button(this);
    max_button      = new State_Button(this);
    close_button    = new State_Button(this);

    //设置name以方便使用 CSS
    icon->setObjectName("title_bar_icon");
    title->setObjectName("title_bar_text");

    menu_button->setObjectName("title_bar_menu");
    min_button->setObjectName("title_bar_min");
    max_button->setObjectName("title_bar_max");
    close_button->setObjectName("title_bar_close");

    init();
}

Title_Bar::~Title_Bar()
{
}


void Title_Bar::init()
{
    //标题栏
    title->setText("这是标题栏");

    // 标题栏高度
    int titlebar_height = style()->pixelMetric(QStyle::PM_TitleBarHeight);
    setFixedHeight(titlebar_height);

    const int frame_width = 5;

    //layout
    titleLayout->addWidget(icon, 0, Qt::AlignVCenter);
    titleLayout->addSpacing(frame_width);
    titleLayout->addWidget(title, 0, Qt::AlignVCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(menu_button, 0, Qt::AlignTop);
    titleLayout->addWidget(min_button, 0, Qt::AlignTop);
    titleLayout->addWidget(max_button, 0, Qt::AlignTop);
    titleLayout->addWidget(close_button, 0, Qt::AlignTop);

    titleLayout->setSpacing(1);
    titleLayout->setMargin(1);

    //icon
    icon->setPixmap(QPixmap(":/icon.png"));
    icon->setFixedSize(16, 16);
    icon->setScaledContents(true);


    //windows taskbar icon
    window()->setWindowIcon(QPixmap(":/icon.png"));
    //
    menu_button->set_pixmap(QPixmap(":/menu.png"));
    min_button->set_pixmap(QPixmap(":/min.png"));
    max_button->set_pixmap(QPixmap(":/max.png"));
    close_button->set_pixmap(QPixmap(":/close.png"));

    //
    setLayout(titleLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //
    connect(min_button, SIGNAL(clicked()), this, SLOT(show_small()));
    connect(max_button, SIGNAL(clicked()), this, SLOT(show_max_restore()));
    connect(close_button, SIGNAL(clicked()), this, SLOT(close_window()));

}

void Title_Bar::show_max_restore()
{
    if (is_maxed)
    {
        is_maxed = false;
        window()->setGeometry(normal_rect);
        max_button->set_pixmap(QPixmap(":/max.png"));
    } else {
        is_maxed = true;
        normal_rect = window()->geometry();
        window()->setGeometry(QApplication::desktop()->availableGeometry());
        max_button->set_pixmap(QPixmap(":/restore.png"));
    }
}

void Title_Bar::mousePressEvent( QMouseEvent *e )
{
    QRect r = rect();
    r.adjust(5, 5, -5, -5);
    if (r.contains(e->pos()))   //5:边框大小，边框用来拉伸改变窗口大小
    {
        if (e->button() == Qt::LeftButton)
        {
            left_pressed = true;
            click_pos = mapToParent(e->pos());
            e->accept();
            return;
        }
    }

    e->ignore();
}

void Title_Bar::mouseMoveEvent( QMouseEvent *e )
{
    if (left_pressed && !is_maxed)
    {
        window()->move(e->globalPos() - click_pos);
        e->accept();
    }else{
        e->ignore();
    }
}

void Title_Bar::mouseReleaseEvent( QMouseEvent *e )
{
    left_pressed = false;
    e->ignore();
}

void Title_Bar::show_small()
{
    window()->showMinimized();
}
void Title_Bar::close_window()
{
    window()->close();
}
void Title_Bar::mouseDoubleClickEvent( QMouseEvent *e )
{
    show_max_restore();
    left_pressed = false;
}


/*---------------------------------*/

frameLessDialog::frameLessDialog(QWidget *parent)
    : QDialog(parent)
{
    setMinimumSize(600,480);
    setWindowFlags( Qt::FramelessWindowHint );
    fh = new NcFramelessHelper;
    fh->activateOn(this);

    QVBoxLayout* layout = new QVBoxLayout;

    Title_Bar* title = new Title_Bar;
    layout->addWidget(title, 0, Qt::AlignTop);
    layout->setMargin(1);

    QFile file(":/dark.qss");
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    setStyleSheet(stream.readAll());



    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setMargin(10);

    QLabel* label = new QLabel("sdf");
    gridLayout->addWidget(label,0,0);

    QPushButton* btn = new QPushButton("sdf");
    gridLayout->addWidget(btn,0,1);

    QCheckBox* check = new QCheckBox("615651651");
    check->setChecked(true);
    gridLayout->addWidget(check,0,2);

    QComboBox* combo = new QComboBox;
    combo->addItem("sfsfsd");
    combo->addItem("234234");
    combo->addItem("78678");
    gridLayout->addWidget(combo,0,3);

    QLineEdit* line = new QLineEdit;
    //line->setFont(QFont("微软雅黑",12));
    line->setText("LineText");
    gridLayout->addWidget(line,0,4);

    QListWidget *listWidget = new QListWidget(this);

    listWidget->insertItem(0,"列表1");
    listWidget->insertItem(1,"列表2");
    listWidget->insertItem(2,"列表3");
    listWidget->insertItem(3,"列表4");
    listWidget->insertItem(4,"列表5");
    listWidget->setAlternatingRowColors(true);
    gridLayout->addWidget(listWidget,1,0,2,2);

    QGroupBox *groupBox = new QGroupBox(tr("按钮组"));

    QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

    radio1->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    gridLayout->addWidget(groupBox,1,2,3,1);

    QTableWidget *table = new QTableWidget(3,2);
    table->setAlternatingRowColors(true);
    table->setWindowTitle("VIP List");
    QStringList headerList;
    headerList << "No." << "姓名" ;
    table->setHorizontalHeaderLabels(headerList);
    //table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);


    table->setItem(0,0,new QTableWidgetItem("0"));
    table->setItem(1,0,new QTableWidgetItem("1"));
    table->setItem(2,0,new QTableWidgetItem("2"));


    table->setItem(0,1,new QTableWidgetItem("小王"));
    table->setItem(1,1,new QTableWidgetItem("小龙"));
    table->setItem(2,1,new QTableWidgetItem("小李"));
    gridLayout->addWidget(table,1,3,4,2);


    layout->addLayout(gridLayout);
    setLayout(layout);
}

frameLessDialog::~frameLessDialog()
{
    delete fh;
}








