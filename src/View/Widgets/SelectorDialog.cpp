#include "SelectorDialog.h"
#include "Presenter/ListSelectorPresenter.h"
#include <QMessageBox>
#include  <QRegularExpression>
#include <QApplication>
#include "View/Theme.h"

SelectorDialog::SelectorDialog(ListSelectorPresenter* presenter) :
	presenter(presenter)
{
	ui.setupUi(this);

	setModal(true);

	setWindowFlags(Qt::Window);
	setWindowTitle("Документи");
	setWindowIcon(QIcon(":/icons/icon_open.png"));

	auto lambda = [=](QDate date) { return Date{ date.day(), date.month(), date.year() };};

	connect(ui.fromDateEdit, &QDateEdit::dateChanged,
		[=]() {presenter->setDates(lambda(ui.fromDateEdit->date()), lambda(ui.toDateEdit->date())); });
	connect(ui.toDateEdit, &QDateEdit::dateChanged,
		[=]() {presenter->setDates(lambda(ui.fromDateEdit->date()), lambda(ui.toDateEdit->date())); });

	connect(ui.openButton, &QPushButton::clicked, [=] {
		
		QApplication::setOverrideCursor(QCursor(Qt::CursorShape::WaitCursor));
		presenter->openCurrentSelection(); 
		QApplication::restoreOverrideCursor();
		
		});

	connect(ui.idSearchEdit, &QLineEdit::textChanged, [=]
		{
			QString text = ui.idSearchEdit->text();
			idFilter.setFilterRegularExpression(QRegularExpression(text, QRegularExpression::PatternOption::CaseInsensitiveOption));
		});

	connect(ui.nameSearchEdit, &QLineEdit::textChanged, [=]
		{
			QString text = ui.nameSearchEdit->text();
			nameFilter.setFilterRegularExpression(QRegularExpression(text, QRegularExpression::PatternOption::CaseInsensitiveOption));
		});

	connect(ui.phoneSearchEdit, &QLineEdit::textChanged, [=]
		{
			QString text = ui.phoneSearchEdit->text();
			phoneFilter.setFilterRegularExpression(QRegularExpression(text, QRegularExpression::PatternOption::CaseInsensitiveOption));
		});



	connect(ui.tableView, &QTableView::doubleClicked, this, [=] { presenter->openCurrentSelection(); });

	connect(ui.tableView, &ListTable::deletePressed, this, [=] { ui.deleteButton->click(); });

	ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.deleteButton, &QPushButton::clicked, 
		[=] 
		{
				presenter->deleteCurrentSelection();
		});


	presenter->setView(this);

}

SelectorDialog::~SelectorDialog()
{
	ui.fromDateEdit->blockSignals(true);
	ui.toDateEdit->blockSignals(true);
	presenter->setView(nullptr);

}


void SelectorDialog::setDates(const Date& from, const Date& to)
{
	QSignalBlocker f(ui.fromDateEdit);
	QSignalBlocker t(ui.toDateEdit);
	ui.fromDateEdit->setDate(QDate(from.year, from.month, from.day));
	ui.toDateEdit->setDate(QDate(to.year, to.month, to.day));
}

void SelectorDialog::setRows(const std::vector<PatientRow>& rows)
{
	patient_model.setRows(rows);

	//QSignalBlocker block(ui.dataTypeCombo);
	//ui.dataTypeCombo->setCurrentIndex(0);
	ui.groupBox->hide();

	idFilter.setSourceModel(&patient_model);
	idFilter.setFilterKeyColumn(1);
	nameFilter.setSourceModel(&idFilter);
	nameFilter.setFilterKeyColumn(2);
	phoneFilter.setSourceModel(&nameFilter);
	phoneFilter.setFilterKeyColumn(3);

	ui.tableView->setModel(&phoneFilter);

	ui.tableView->hideColumn(0);
	
	ui.tableView->setColumnWidth(1, 80);
	ui.tableView->setColumnWidth(2, 250);
	ui.tableView->setColumnWidth(3, 120);
	

	ui.fromDateEdit->setDisabled(true);
	ui.toDateEdit->setDisabled(true);


	connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=] {

		auto idxList = ui.tableView->selectionModel()->selectedRows();

		std::set<int>selectedIndexes;

		for (auto& idx : idxList) {
			selectedIndexes.insert(phoneFilter.index(idx.row(), 0).data().toInt());
		}

		this->presenter->selectionChanged(selectedIndexes);


		}

	);
}

void SelectorDialog::setRows(const std::vector<PrescriptionRow>& rows)
{
	perscription_model.setRows(rows);

	ui.groupBox->show();

	idFilter.setSourceModel(&perscription_model);
	idFilter.setFilterKeyColumn(3);
	nameFilter.setSourceModel(&idFilter);
	nameFilter.setFilterKeyColumn(4);
	phoneFilter.setSourceModel(&nameFilter);
	phoneFilter.setFilterKeyColumn(5);

	ui.tableView->setModel(&phoneFilter);

	ui.tableView->hideColumn(0);
	ui.tableView->setColumnWidth(1, 100);
	ui.tableView->setColumnWidth(2, 80);
	ui.tableView->setColumnWidth(3, 250);
	ui.tableView->setColumnWidth(4, 120);

	ui.fromDateEdit->setDisabled(false);
	ui.toDateEdit->setDisabled(false);

	connect(ui.tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=] {

		auto idxList = ui.tableView->selectionModel()->selectedRows();

		std::set<int>selectedIndexes;

		for (auto& idx : idxList) {
			selectedIndexes.insert(phoneFilter.index(idx.row(), 0).data().toInt());
		}

		this->presenter->selectionChanged(selectedIndexes);

		}

	);
}



void SelectorDialog::focus()
{
	activateWindow();
}

void SelectorDialog::close()
{
	QDialog::accept();
}

