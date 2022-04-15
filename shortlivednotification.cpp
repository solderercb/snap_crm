#include "shortlivednotification.h"

QVector<shortlivedNotification*> shortlivedNotification::p_instance;
int shortlivedNotification::popupsCounter;
QSize shortlivedNotification::size = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
QSize shortlivedNotification::currentScreen = {1366, 738}; // размер экрана по умолчанию (без панели задач)
QPoint shortlivedNotification::appearance = {0,0};
int shortlivedNotification::appearanceStep = DEFAULT_HEIGHT + 5;
QFont shortlivedNotification::captionFont = QFont("Calibri", 12, QFont::Bold);
QFont shortlivedNotification::messageFont = QFont("Calibri", 11);
QColor shortlivedNotification::captionColor = QColor(255,255,255);    // Цвета по умолчанию, белый
QColor shortlivedNotification::messageColor = QColor(245,245,245);    // светло-серый
int shortlivedNotification::maxDisplayedNotifications = MAX_POPUPS;

// captionColor_ и messageColor_ имеют приоритет над static captionColor и static messageColor;
// вторая пара цветов может быть настроена с помощью setCaptionColor(QColor) и setMessageColor(QColor) соответственно, и все следующие уведомления по умолчанию будут иметь эти цвета
shortlivedNotification::shortlivedNotification(QWidget *parent, QString caption, QString message, QColor captionColor_, QColor messageColor_, int millisecondsTimeout):
    QWidget()
{
    const QString labelCaptionStyle =   "border: 0px;\
                                        border-top: 1px solid gray;\
                                        border-left: 1px solid gray;\
                                        border-right: 1px solid gray;\
                                        border-radius: 0px;\
                                        border-top-left-radius: 10px;\
                                        border-top-right-radius: 10px;\
                                        padding-left: 7px;";

    const QString labelMessageStyle =  "border: 0px;\
                                        border-bottom: 1px solid gray;\
                                        border-left: 1px solid gray;\
                                        border-right: 1px solid gray;\
                                        border-radius: 0px;\
                                        border-bottom-left-radius: 10px;\
                                        border-bottom-right-radius: 10px;\
                                        margin-top: 24px;\
                                        padding-left: 7px;\
                                        padding-right: 7px;\
                                        padding-bottom: 3px;";
    if( p_instance.size() >= MAX_POPUPS )
    {
      p_instance.removeFirst();
//      repaintPopups();
    }
    p_instance.append(this);
    popupsCounter++;

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    repaintPopups();
//    qDebug() << "size = " << size;
//    qDebug() << "shortlivedNotification: QApplication::screenAt(parent->pos()):" << parent->pos() << "; availableSize():" << currentScreen;
//    qDebug() << "shortlivedNotification: QWidget::mapToGlobal(QPoint(0,0)): " << QWidget::mapToGlobal(QPoint(0,0)); // QPoint(0,0) with respect to this
    setAttribute(Qt::WA_TranslucentBackground);
    labelCaption = new label(this);
    labelCaption->setObjectName(QString::fromUtf8("labelCaption"));
    labelCaption->setGeometry(QRect(0, 0, size.width(), 24));
    labelCaption->setFont(captionFont);
    labelCaption->setStyleSheet(QString(  "QLabel {background-color: %1;" + labelCaptionStyle + "}").arg(captionColor_.name()));
    labelMessage = new label(this);
    labelMessage->setObjectName(QString::fromUtf8("labelMessage"));
    labelMessage->setGeometry(QRect(0, 0, size.width(), size.height()));
    labelMessage->setFont(messageFont);
    labelMessage->setStyleSheet(QString(  "QLabel {background-color: %1;" + labelMessageStyle + "}").arg(messageColor_.name()));
    labelMessage->setLineWidth(1);
    labelMessage->setScaledContents(true);
    labelMessage->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    labelMessage->setWordWrap(true);
    labelMessage->setMargin(0);
    labelMessage->setTextInteractionFlags(Qt::NoTextInteraction);

    labelMessage->setText(message);
    labelCaption->setText(caption);

    autoDisappearTimer = new QTimer();
    autoDisappearTimer->setSingleShot(true);
    animatedPaintTimer = new QTimer();
    animatedPaintTimer->setSingleShot(true);
    connect(autoDisappearTimer, SIGNAL(timeout()), this, SLOT(autoDisappearTimer_handler()));
    connect(animatedPaintTimer, SIGNAL(timeout()), this, SLOT(animatedPaintTimer_handler()));
    connect(labelCaption, SIGNAL(clicked()), this, SLOT(close()));
    connect(labelMessage, SIGNAL(clicked()), this, SLOT(close()));

    autoDisappearTimer->start(millisecondsTimeout);
}

shortlivedNotification::~shortlivedNotification()
{
    delete labelCaption;
    delete labelMessage;
    p_instance.removeOne(this);
    repaintPopups();
}

void shortlivedNotification::repaintPopups()
{
    for (int i = 0; i < p_instance.size(); i++)
    {
        p_instance.at(i)->setGeometry(QRect(appearance.x(), appearance.y() + appearanceStep*i, size.width(), size.height()));
//        qDebug() << "shortlivedNotification::repaintPopups(): currentScreen:" << currentScreen;
//        qDebug() << "shortlivedNotification::repaintPopups(): popupGeometry:" << QRect(currentScreen.width() - size.width(), currentScreen.height() - size.height() - i*95, size.width(), size.height());
    }

}

QSize shortlivedNotification::sizeHint()
{
    return QSize(385, DEFAULT_HEIGHT);
}

void shortlivedNotification::setSize(QSize newSize)
{
    size = newSize;
}

void shortlivedNotification::setSize(int widht, int height)
{
    size = QSize(widht, height);
}

void shortlivedNotification::setAppearance(QWidget *parent, int corner)
{
    //    QScreen screen;
    //    QSize screenSize = QScreen::availableSize();
    currentScreen = QApplication::screenAt(QPoint(parent->pos().x() + 10, parent->pos().y() + 10))->availableSize(); // TODO: проработать механизм отрисовки всплывающих сообщений на том экране, где находится приложение
    switch (corner) {
        case topLeft:  appearance = {0, 0}; appearanceStep = size.height() + 5; break;
        case topRight:  appearance = {currentScreen.width() - size.width(), 0}; appearanceStep = size.height() + 5; break;
        case bottomLeft:  appearance = {0, currentScreen.height() - size.height()}; appearanceStep = -size.height() - 5; break;
        default:  appearance = {currentScreen.width() - size.width(), currentScreen.height() - size.height()}; appearanceStep = -size.height() - 5; break;
    };
}

void shortlivedNotification::setCaptionFont(QFont font)
{
    captionFont = font;
}

void shortlivedNotification::setMessageFont(QFont font)
{
    messageFont = font;
}

void shortlivedNotification::setCaptionColor(QColor color)
{
    captionColor = color;
}

void shortlivedNotification::setMessageColor(QColor color)
{
    messageColor = color;
}

void shortlivedNotification::setMaxDisplayedNotifications(int count)
{
    maxDisplayedNotifications = count;
}

void shortlivedNotification::close()
{
    this->deleteLater();
}

void shortlivedNotification::autoDisappearTimer_handler()  // обработик таймера закрытия уведомления
{
//    qDebug() << "autoDisappearTimer_handler()";
    close();
}

void shortlivedNotification::animatedPaintTimer_handler()  // TODO: обработик таймера анимированного закрытия уведомления
{
//    qDebug() << "animatedPaintTimer_handler()";
}

label::label(QWidget *parent) :
    QLabel(parent)
{

}

label::~label()
{

}

void label::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
