#pragma once
#include <unordered_set>
#include <string>
#include <vector>


enum class SelectionPref {All, OnlyNZOK, OnlyPaid};
enum class MonthNotifLoad {FromPIS, FromFile, Rejected};


class PatientDialogPresenter;
class ListSelectorPresenter;
class MedicationPresenter;
class DosagePresenter;
class DoctorDialogPresenter;

enum class DialogAnswer { Yes, No, Cancel };

enum class NotificationType {NoData, Xml, PisHash };


namespace ModalDialogBuilder
{
	void openDialog(PatientDialogPresenter* p);
	void openDialog(ListSelectorPresenter* p);
	void openDialog(MedicationPresenter* p);
	void openDialog(DosagePresenter* p);
	void openDialog(DoctorDialogPresenter* p);
	DialogAnswer openSaveDialog(const std::string& text);

	//returns true if user wants to apply the procedures to the status
	bool askDialog(const std::string& questionText);
	void showError(const std::string& error);

	void showMessage(const std::string& message);
	std::string pinPromptDialog(const std::string& pem);
	std::string inputDialog(const std::string& text, const std::string& title, bool asPassword = false);

	void show_eIDAS_Warning();
};

