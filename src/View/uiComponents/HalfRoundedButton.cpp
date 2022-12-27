#include "HalfRoundedButton.h"
#include <QEvent>
#include <QPainter>
#include <QApplication>

#include "View/Theme.h"

bool HalfRoundedButton::eventFilter(QObject* obj, QEvent* e)
{
	if (e->type() == QEvent::HoverEnter) {
		QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
		m_hover = true;
		update();
	}

	if (e->type() == QEvent::HoverLeave) {
		QApplication::restoreOverrideCursor();
		m_hover = false;
		update();
	}

	return false;
}

HalfRoundedButton::HalfRoundedButton(QWidget *parent)
	: QPushButton(parent)
{
	this->installEventFilter(this);
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	auto f = font();
	f.setBold(true);
	setFont(f);
}

void HalfRoundedButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing);
	QColor color{ isChecked() ? Theme::background : Theme::sectionBackground };

	if (m_hover && !isChecked())
	{
		color = Theme::background;
	}
	

	QPen pen(Theme::fontTurquoiseClicked);
	pen.setCosmetic(true);
	pen.setWidth(2);
	painter.setPen(pen);

	QPainterPath path;

	switch (m_position)
	{
	case Position::Alone: path.addRoundedRect(0, 0, width(), height(), 8, 8); break;
	case Position::Center: path.addRect(this->rect()); break;
	case Position::Left: path = Theme::getHalfCurvedPath(width(), height()); break;
	case Position::Right:
		{
			path = Theme::getHalfCurvedPath(width(), height());
			QTransform mirror(-1, 0, 0, 0, 1, 0, 0, 0, 1);
			painter.setTransform(mirror);
			painter.translate(-width(), 0);
			break;
		}

	}

	painter.fillPath(path, color);
	//painter.drawPath(path);

	painter.resetTransform();


	painter.setPen(isChecked() || m_hover ? Theme::fontTurquoise : Theme::fontTurquoiseClicked);

	painter.drawText(rect(), Qt::AlignCenter, text());

	painter.end();
}


HalfRoundedButton::~HalfRoundedButton()
{
}
