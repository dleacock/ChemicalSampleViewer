/*
 *  SampleView
 *
 *  Written to fulfill requirments for Software Analyst Project.
 *  All code is designed, written and edited by David Leacock,
 *  from May 21st and completed May 30th.
*/

#ifndef SAMPLEVIEW_H
#define SAMPLEVIEW_H

#include <QWidget>

class QDataWidgetMapper;
class QLabel;
class QTextEdit;
class QLineEdit;
class QStandardItemModel;
class QListView;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QGroupBox;

class SampleView : public QWidget {
    Q_OBJECT

public:
    /// Constructor.
    SampleView(QWidget *parent = 0);
    ~SampleView();

private slots:
    /// Update sample name in model. This is used for dynamically updating model.
    void updateSampleName(QString name);
    /// Update sample chemical symbol in model. This is used for dynamically updating model.
    void updateSampleChemSymbol(QString name);
    /// Popup new dialog window to create new sample.
    void addNewSampleDialog();
    /// Helper function to setup layouts and widget for new sample dialog.
    void setupAddSampleDialog();
    /// Helper function to validate sample to prevent samples with blank names.
    void validateNewSampleInfo();
    /// Add the new sample to the model.
    void addSampleToModel(QString name, QString chem, QString notes);
    /// Remove the currently selected sample from the model.
    void removeSample();

private:
    /// Simple helper function to setup model. Would be an appropriate place to pre-populate model if users requested such.
    void setupModel();
    /// Helper function to create widgets and layout for main program.
    void setupMainLayout();
    /// Setup connects for add/removal buttons, connecting sampleListView to data mapper widget and updating sample name/symbol methods.
    void setupMainConnections();

    /// Currently selected sample name.
    QLabel *nameLabel_;
    QLineEdit *nameEdit_;
    /// Currently selected sample chemical symbol.
    QLabel *chemLabel_;
    QLineEdit *chemEdit_;
    /// Currently selected sample notes.
    QLabel *notesLabel_;
    QTextEdit *notesEdit_;
    /// List view for model class. Allows users to select model.
    QListView *sampleListView_;
    /// Buttons for adding new sample, or removing selected sample.
    QPushButton *addSampleButton_;
    QPushButton *removeSampleButton_;

    /// Model object that represents samples. Each sample contains a name, a chemical symbol and notes plus a title to be displayed.
    /// Only the sample name is required. The other fields can remain blank.
    QStandardItemModel *sampleModel_;
    /// Mapping object that allows the selected model item in the sampleListView to populate the appropriate QLineEdit and QTextEdit.
    QDataWidgetMapper *modelToWidgetMapper_;

    /// Master layout.
    QHBoxLayout *sampleWidgetLayout_;
    /// Groupbox layout for sample list
    QGroupBox *sampleListGroupBox_;
    /// Layout for sample list, used for groupbox.
    QVBoxLayout *sampleListLayout_;

    /// Layout for all sample info widgets.
    QVBoxLayout *sampleInfoLayout_;
    /// Layout for individual info.
    QHBoxLayout *sampleNameLayout_;
    QHBoxLayout *sampleChemLayout_;
    QHBoxLayout *sampleNotesLayout_;
    /// Layout for add/remove buttons.
    QHBoxLayout *addRemoveButtonLayout_;

    /// Dialog window that opens upon pressing addSampleButton_.
    QDialog *addSampleDialog_;

    /// New sample name.
    QLabel *newNameLabel_;
    QLineEdit *newNameEdit_;
    /// New sample chemical symbol.
    QLabel *newChemLabel_;
    QLineEdit *newChemEdit_;
    /// New sample notes.
    QLabel *newNotesLabel_;
    QTextEdit *newNotesEdit_;
    /// Confirm sample add / Cancel sample add buttons.
    QPushButton *confirmAddSampleButton_;
    QPushButton *cancelSampleAdd_;

    /// New sample dialog window layouts.
    QHBoxLayout *addNameLayout_;
    QHBoxLayout *addChemLayout_;
    QHBoxLayout *addNotesLayout_;
    QVBoxLayout *addSampleInfoLayout_;

    QHBoxLayout *addButtonLayout_;

};

#endif // SAMPLEVIEW_H
