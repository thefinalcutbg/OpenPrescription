#include "TileButton.h"
#include <QPainterPath>
#include "View/Theme.h"
#include <QApplication>

TileButton::TileButton(QWidget* parent) : QAbstractButton(parent)
{
	header.setFamily("Segoe UI semibold");
	header.setPointSizeF(14);

	header.setBold(1);
	info.setPointSizeF(8);
	info.setFamily("Segoe UI");
	
	infoLabel.setFamily("Segoe UI");
	infoLabel.setBold(true);
	infoLabel.setPointSizeF(8);

	this->installEventFilter(this);

}

void TileButton::paintEvent(QPaintEvent* e)
{


	//if(hover) color.setRgb(242, 242, 242);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	//getting the half-rounded button path:

	QPainterPath path;
	path.addRoundedRect(0, 0, width(), height(), Theme::radius, Theme::radius);

	if(m_reveresed) //transforming if reversed;
	{
		QTransform mirror(-1, 0, 0, 0, 1, 0, 0, 0, 1);
		painter.setTransform(mirror);
		painter.translate(-width(), 0);
	}

	painter.fillPath(path, Theme::sectionBackground);

	QPen pen(Theme::border);
	pen.setCosmetic(true);
	pen.setWidth(2);
	painter.setPen(pen);

	painter.drawPath(path);

	if (m_reveresed)
	{
		painter.resetTransform();
	}


	QColor textColor = hover && !clicked ?
		QColor(Theme::fontTurquoiseClicked)
		:
		QColor(Theme::fontTurquoise);

	painter.setPen(QPen(textColor));

	paintInfo(&painter);

	painter.end();
}

bool TileButton::eventFilter(QObject* obj, QEvent* e)
{
	if (e->type() == QEvent::HoverEnter) {
		hover = true;
		QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
		update();
	}

	if (e->type() == QEvent::HoverLeave) {
		QApplication::restoreOverrideCursor();
		hover = false;
		update();
	}

	if (e->type() == QEvent::MouseButtonPress)
	{
		clicked = true;
	}

	if (e->type() == QEvent::MouseButtonRelease)
	{
		clicked = false;
	}

	return false;
}

QString TileButton::elide(const QString& text, int length)
{
	if (text.length() < length) 
		return text;
	
	return text.chopped(text.length() - length) + "...";
}



PatientTile::PatientTile(QWidget* parent) : TileButton(parent)
{

}


void PatientTile::paintInfo(QPainter* painter)
{
	QFontMetrics fm(info);

	int phonePosX = width() - fm.horizontalAdvance(phone) - 10;
	int addressPosX = width() - fm.horizontalAdvance(address) - 10;

	constexpr int rowYPos[3]{ 60,80,100 };

	painter->setFont(infoLabel);
	painter->drawText(20, rowYPos[0], idLabel);
	painter->drawText(20, rowYPos[1], "Рождена дата: ");
	painter->drawText(20, rowYPos[2], "Възраст: ");

	painter->drawText(width() / 2, rowYPos[0], "Телефон: ");
	painter->drawText(width() / 2, rowYPos[1], "Адрес: ");


	QFontMetrics metric(infoLabel);

	auto horizontalAdvance = [metric](const QString& label) {
		return metric.horizontalAdvance(label);
	};

	painter->setFont(info);
	painter->drawText(20 + horizontalAdvance(idLabel), rowYPos[0], id);
	painter->drawText(20 + horizontalAdvance("Рождена дата: "), rowYPos[1], birthDate);
	painter->drawText(20 + horizontalAdvance("Възраст: "), rowYPos[2], age);

	painter->drawText(width() / 2 + horizontalAdvance("Телефон: "), rowYPos[0], phone);
	painter->drawText(width() / 2 + horizontalAdvance("Адрес: "), rowYPos[1], address);


	painter->setFont(header);
	painter->setPen(hover && !clicked ? QPen(Theme::fontRedClicked) : QPen(QColor(Theme::fontRed)));
	painter->drawText(20, 27, name);


}


void PatientTile::setData(const Patient& patient, int age)
{
	name = elide(QString::fromStdString(patient.fullName()), 35);

	if (patient.type == 1) idLabel = "ЕГН: ";
	else idLabel = "ЛНЧ: ";

	id = QString::fromStdString(patient.id);

	birthDate = QString::fromStdString((patient.birth.toBgStandard(true)));

	this->age = QString::number(age) + " г.";

	address = elide(QString::fromStdString(patient.getFullAddress()), 35);

	if (patient.phone != "")
		phone = QString::fromStdString(patient.phone);
	else phone = "";

	update();
}

