﻿#include "widget.h"
#include "ui_widget.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
//#include <<QGraphics

#include <QFileDialog>
#include <QDebug>
#include <QWheelEvent>

#include <qmath.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_graphicsScene(new QGraphicsScene(this)),
    m_scale(0.3)
{
    ui->setupUi(this);

//    m_graphicsScene->setSelectionArea(0,0, 1920*m_scale*2, 1080*m_scale);
    m_graphicsScene->setSceneRect(0,0, 1920*m_scale*2, 1080*m_scale);
    m_graphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_graphicsView = ui->graphicsView;
    ui->graphicsView->setScene(m_graphicsScene);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::QGraphicsView::AnchorViewCenter);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);
    ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_graphicsView->installEventFilter(this);

    // Prepare background check-board pattern
    QPixmap tilePixmap(200, 200);
    tilePixmap.fill(Qt::white);
    QPainter tilePainter(&tilePixmap);
    QColor color(222,222,222,222);
    tilePainter.fillRect(0, 0, 100, 100, color);
    tilePainter.fillRect(100, 100, 100, 100, color);
    tilePainter.end();
    m_graphicsView->setBackgroundBrush(tilePixmap);
//    m_graphicsScene->setBackgroundBrush(tilePixmap);

    auto pixmapItem = new QGraphicsPixmapItem(tilePixmap);
    pixmapItem->setTransformationMode(Qt::SmoothTransformation);
    m_imageItem = pixmapItem;
    if (!m_imageItem) // failed to load
        return;
    m_imageItem->setCacheMode(QGraphicsItem::NoCache);
    m_imageItem->setZValue(0);
    // background item
    m_backgroundItem = new QGraphicsRectItem(m_imageItem->boundingRect());
    m_backgroundItem->setBrush(Qt::white);
    m_backgroundItem->setPen(Qt::NoPen);
    m_backgroundItem->setVisible(true);
    m_backgroundItem->setZValue(-1);
    // outline
    m_outlineItem = new QGraphicsRectItem(m_imageItem->boundingRect());
    QPen outline(Qt::black, 6, Qt::DashLine);
    outline.setCosmetic(true);
    m_outlineItem->setPen(outline);
    m_outlineItem->setBrush(Qt::NoBrush);
    m_outlineItem->setVisible(true);
    m_outlineItem->setZValue(1);
    QGraphicsScene *s = m_graphicsView->scene();
    s->addItem(m_backgroundItem);
    s->addItem(m_imageItem);
    s->addItem(m_outlineItem);

    QGraphicsRectItem *rectItem1 = (new QGraphicsRectItem(0,0, 1920*m_scale, 1080*m_scale));
    QPen pen1(QColor(Qt::blue));
    pen1.setWidth(3);
    rectItem1->setPen(pen1);
    rectItem1->setScale(1);
    rectItem1->setSelected(true);
    rectItem1->setActive(true);
    rectItem1->setEnabled(true);
    m_graphicsScene->addItem(rectItem1);

    QGraphicsRectItem *rectItem2 = (new QGraphicsRectItem(1920*m_scale,0, 1920*m_scale, 1080*m_scale));
    QPen pen2(QColor(Qt::red));
    pen2.setWidth(3);
    rectItem2->setPen(pen2);
    rectItem2->setScale(1);
    rectItem2->setSelected(true);
    rectItem2->setActive(true);
    rectItem2->setEnabled(true);
    m_graphicsScene->addItem(rectItem2);


    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(0,0, 1920*m_scale, 1080*m_scale);
    QPen pen3(QColor(Qt::yellow));
    pen3.setWidth(2);
    pen3.setStyle(Qt::DashLine);
    ellipseItem->setPen(pen3);
    ellipseItem->setAcceptedMouseButtons(Qt::LeftButton);
    m_graphicsScene->addItem(ellipseItem);


    QGraphicsLineItem *lineItem = new QGraphicsLineItem(1920*m_scale, 1080*m_scale*0.5, 1920*m_scale*2, 1080*m_scale*0.5);
    QPen pen4(QColor(Qt::red));
    pen4.setWidth(1);
    pen4.setStyle(Qt::DashLine);
    lineItem->setPen(pen4);
    m_graphicsScene->addItem(lineItem);

    QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(1920*m_scale*1.5, 0, 1920*m_scale*1.5, 1080*m_scale);
    QPen pen5(QColor(Qt::red));
    pen5.setWidth(1);
    pen5.setStyle(Qt::DashLine);
    lineItem2->setPen(pen5);
    m_graphicsScene->addItem(lineItem2);

    QPainterPath path;
    path.addRect(20, 20, 60, 60);
    path.moveTo(0, 0);
    path.cubicTo(150, 0,  150, 150,  300, 300);
    path.cubicTo(150, 300,  150, 150,  0, 0);
    QGraphicsPathItem *pathItem = new QGraphicsPathItem(path);
    m_graphicsScene->addItem(pathItem);

    QGraphicsPixmapItem *p  = new QGraphicsPixmapItem(QPixmap(":/C:/Users/86176/Desktop/1568944190(1).png"), rectItem1);
    p->setEnabled(true);
    p->setActive(true);
    p->setScale(0.8);
    p->setPos(0, 50);
    m_graphicsScene->addItem(p);

    QPolygon polygon;
    polygon << QPoint(1920*m_scale,0) << QPoint(1920*m_scale+10,100) << QPoint(1920*m_scale+50,100);
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(polygon,rectItem2);
    m_graphicsScene->addItem(polygonItem);

    QGraphicsSimpleTextItem *simpleTextItem = new QGraphicsSimpleTextItem(QString("QGraphicsSimpleTextItem"), rectItem2);
    simpleTextItem->setAcceptDrops(true);
    simpleTextItem->setPos(1920*m_scale,150);
    m_graphicsScene->addItem(simpleTextItem);

    QGraphicsTextItem *textItem = new QGraphicsTextItem(QString("QGraphicsTextItem"), rectItem2);
    textItem->setHtml(QString("www.baidu.com"));
    textItem->setSelected(true);
    textItem->setPos(1920*m_scale*1.5,150);
    m_graphicsScene->addItem(textItem);

    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDragDropMode(QAbstractItemView::DragDrop);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_graphicsView)
    {
        qDebug() << "Widget::eventFilter";
        if (event->type() == QEvent::Wheel)
        {
            qDebug() << "event->type() == QEvent::Wheel";
            QWheelEvent * we = dynamic_cast<QWheelEvent *>(event);
            qDebug() << "we->delta() ：" << we->delta();
            qDebug() << "we->angleDelta()" << we->angleDelta();
            qDebug() << "we->pixelDelta()" << we->pixelDelta();
            qreal factor = qPow(1.2, we->delta() / 240.0);
            qDebug() << "factor = qPow(1.1, we->delta() / 240.0);" << qPow(1.1, we->delta() / 240.0);
            qreal currentScale = m_graphicsView->transform().m11();
            qDebug() << "currentScale=m_graphicsView->transform().m11()" << m_graphicsView->transform().m11();
            qreal newScale = currentScale * factor;
             qDebug() << "newScale = currentScale * factor;" << currentScale * factor;;
            qreal actualFactor = factor;
            // cap to 0.001 - 1000
            if (newScale > 1000)
                actualFactor = 1000./currentScale;
            else if (newScale < 0.001)
                actualFactor = 0.001/currentScale;
            m_graphicsView->scale(actualFactor, actualFactor);
            qDebug() << "actualFactor" << actualFactor;
//            m_graphicsScene->items().at(0)->setScale(actualFactor);
//            m_graphicsScene->items().at(1)->setScale(actualFactor);
            event->accept();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);

}
