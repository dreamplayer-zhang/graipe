/************************************************************************/
/*                                                                      */
/*               Copyright 2008-2017 by Benjamin Seppke                 */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the GrAphical Image Processing Enviroment.   */
/*    The GRAIPE Website may be found at:                               */
/*        https://github.com/bseppke/graipe                             */
/*    Please direct questions, bug reports, and contributions to        */
/*    the GitHub page and use the methods provided there.               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#include "qgraphicsresizableitem.hxx"

#include <cmath>

#include <QCursor>
#include <QtDebug>

namespace graipe {

/**
 * @addtogroup graipe_core
 * @{
 *     @file
 *     @brief Implementation file for the QGraphicsResizableItem class
 * @}
 */
    
/** codes for the handles of the Resizable items **/
static const unsigned short top_c = 1, 
							right_c = 2, 
							bottom_c = 4, 
							left_c = 8, 
							forbidden_c = 16;


QGraphicsResizableItem::QGraphicsResizableItem(const QRectF& rect, unsigned int handle_size, bool protect_h_scaling, bool protect_v_scaling, QGraphicsItem * parent)
:	QGraphicsRectItem(rect, parent),
	m_handle_size(handle_size),
	m_protect_h_scaling(protect_h_scaling), 
	m_protect_v_scaling(protect_v_scaling)
{
    m_pen.setWidth(0);
    
    setFlags(ItemIsSelectable);
	setAcceptHoverEvents(true);
}

void QGraphicsResizableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    painter->save();
    
	if(isSelected())
	{
		painter->setPen(m_pen);
        
		painter->setBrush(Qt::transparent);
		painter->drawRect(rect());
			
		painter->setBrush(Qt::black);		
		painter->drawRect(QRectF(rect().center().x() - m_handle_size/2.0,   rect().top(),                               m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().right() - m_handle_size,            rect().top(),                               m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().right() - m_handle_size,            rect().center().y() - m_handle_size/2.0,    m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().right() - m_handle_size,            rect().bottom() - m_handle_size,            m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().center().x() - m_handle_size/2.0,   rect().bottom() - m_handle_size,            m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().left(),                             rect().bottom() - m_handle_size,            m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().left(),                             rect().center().y() - m_handle_size/2.0,    m_handle_size, m_handle_size));
		painter->drawRect(QRectF(rect().left(),                             rect().top(),                               m_handle_size, m_handle_size));
	}
	painter->restore();
}

void QGraphicsResizableItem::protectHScaling(bool protect)
{
	m_protect_h_scaling = protect; 
}

void QGraphicsResizableItem::protectVScaling(bool protect)
{
	m_protect_v_scaling = protect; 
}

void QGraphicsResizableItem::protectScaling(bool protect_h, bool protect_v)
{
	protectHScaling(protect_h);
	protectVScaling(protect_v);
}

void QGraphicsResizableItem::setRect(const QRectF& rect)
{
    if(    rect.width()>=m_handle_size*2
       &&  rect.height()>=m_handle_size*2)
    {
        QGraphicsRectItem::setRect(rect);
    }
}

void QGraphicsResizableItem::setRect(qreal l, qreal t, qreal w, qreal h)
{
    if(    w>=m_handle_size*2
       &&  h>=m_handle_size*2)
    {
        QGraphicsRectItem::setRect(l,t,w,h);
    }
}

void QGraphicsResizableItem::updateRect(const QRectF & new_rect)
{
	if(m_protect_h_scaling && new_rect.width()!=rect().width())
	{
		//forbidden
	}
	else if(m_protect_v_scaling && new_rect.height()!=rect().height())
	{
		//also forbidden
	}
	else
    {
        prepareGeometryChange();
        setRect(new_rect);
        update();
	}
}

void QGraphicsResizableItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    m_dragStart=event->pos();
    
    float	delta_l = std::abs(m_dragStart.x()-rect().left()),
			delta_t = std::abs(m_dragStart.y()-rect().top()),
			delta_r = std::abs(m_dragStart.x()-rect().right()),
			delta_b = std::abs(m_dragStart.y()-rect().bottom());
	
	m_handle=0;
	
	//look left:
	if (delta_l < m_handle_size)
	{
		if( m_protect_h_scaling )
		{
			setCursor(Qt::ForbiddenCursor);
			m_handle = forbidden_c;
		}
		else
		{
			m_handle = m_handle | left_c ;
			setCursor(Qt::SizeHorCursor);
		}
	}
	
	if (delta_r < m_handle_size)
	{
		if( m_protect_h_scaling )
		{
			setCursor(Qt::ForbiddenCursor);
			m_handle =  forbidden_c;
		}
		else
		{
			m_handle = m_handle | right_c;
			setCursor(Qt::SizeHorCursor);
		}
	}
	
	if (delta_t < m_handle_size)
	{
		if( m_protect_v_scaling )
		{
			setCursor(Qt::ForbiddenCursor);
			m_handle = forbidden_c;
		}
		else
		{
			m_handle = m_handle | top_c;
			
			if(m_handle & right_c)
			{
            	setCursor(Qt::SizeBDiagCursor);
			}
            else if(m_handle & left_c)
			{
            	setCursor(Qt::SizeFDiagCursor);
			}
            else
			{
            	setCursor(Qt::SizeVerCursor);
            }
        }
	}
	
	if (delta_b < m_handle_size)
	{
		if( m_protect_v_scaling )
		{
			setCursor(Qt::ForbiddenCursor);
			m_handle = forbidden_c;
		}
		else
		{
			m_handle = m_handle | bottom_c;
			
			if(m_handle & right_c)
			{
            	setCursor(Qt::SizeFDiagCursor);
			}
            else if(m_handle & left_c)
			{
            	setCursor(Qt::SizeBDiagCursor);
			}
            else
			{
            	setCursor(Qt::SizeVerCursor);
            }
        }
	}
	
	if(!m_handle)
	{
    	setCursor(Qt::SizeAllCursor);
    }
    
	QGraphicsItem::mousePressEvent(event);	
}

void QGraphicsResizableItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	setCursor(Qt::ArrowCursor);
	QGraphicsItem::mouseReleaseEvent(event);
}

void QGraphicsResizableItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{		
	if (m_handle != forbidden_c)
	{
        QRectF new_rect(rect());
	
        if( m_handle & left_c)
        {
            new_rect.setLeft(event->pos().x());
        }
        if( m_handle & top_c)
        {
            new_rect.setTop(event->pos().y());
        }
        if( m_handle & right_c)
        {
            new_rect.setRight(event->pos().x());
        }
        if( m_handle & bottom_c)
        {
            new_rect.setBottom(event->pos().y());
        }
        if( m_handle != 0)
        {
            updateRect(new_rect);
        }
        else
        {
            QPointF delta = event->pos() - m_dragStart;
			prepareGeometryChange();
            moveBy(delta.x(), delta.y());
        }
        event->accept();
    }
	else
    {
		QGraphicsItem::mouseMoveEvent(event);
	}
}

}//end of namespace graipe
