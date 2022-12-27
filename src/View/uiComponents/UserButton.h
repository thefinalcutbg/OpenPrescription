#pragma once

#include <QPushButton>

class UserButton  : public QPushButton
{
	Q_OBJECT

	bool m_hover{ false };
	bool m_clicked{ false };
	QColor hoverColor;
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* e) override;
	QFont m_font;



public:
	UserButton(QWidget *parent);
	void setHoverColor(const QColor& color);
	~UserButton();
};
