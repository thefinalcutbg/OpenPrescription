#include "SpecialtyDialog.h"

SpecialtyDialog::SpecialtyDialog(const QString& name, QWidget* parent) :
	 QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Window);

	m_proxyModel.setSourceModel(&m_model);
	m_proxyModel.setFilterKeyColumn(1);

	ui.tableView->setModel(&m_proxyModel);

	ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.tableView->horizontalHeader()->setHighlightSections(false);
	ui.tableView->verticalHeader()->setVisible(false);
	ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableView->verticalHeader()->setDefaultSectionSize(20);
	ui.tableView->setWordWrap(false);
	ui.tableView->setColumnWidth(0, 70);
	//ui.tableView->setColumnWidth(1, 200);
	ui.tableView->setColumnWidth(2, 150);

	ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);


	connect(ui.searchEdit, &QLineEdit::textChanged, [=](const QString& text)
		{
			m_proxyModel.setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
			ui.tableView->selectRow(m_selectedRow);
		});

	connect(ui.cancelButton, &QPushButton::clicked, [=] { reject(); });
	connect(ui.okButton, &QPushButton::clicked, [=] { 

		m_selectedRow = ui.tableView->selectionModel()->currentIndex().row();
		if (m_selectedRow > -1) accept(); 
		
	});
	connect(ui.tableView, &QTableView::doubleClicked, [=] { ui.okButton->click(); });

	setWindowTitle("Избор на специалност");

	ui.tableView->selectRow(m_model.getRow(name));
}


std::string SpecialtyDialog::getResult()
{
	if(result() == QDialog::Rejected) return {};

	return ui.tableView->model()->index(m_selectedRow, 1)
		.data()
		.toString()
		.toStdString();

	
}

SpecialtyDialog::~SpecialtyDialog()
{}
