#include <QDataWidgetMapper>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>

#include "sampleview.h"

SampleView::SampleView(QWidget *parent)
    : QWidget(parent) {

    // Add sample dialog will be instansiated upon Add Sample request.
    addSampleDialog_ = 0;

    setupModel();
    setupMainLayout();
    setupMainConnections();

    setWindowTitle("Sample Manager 1.0 - David Leacock");
}

void SampleView::setupMainConnections(){

    // Pop-up add sample dialog when add sample button clicked.
    connect(addSampleButton_, SIGNAL(clicked()), this, SLOT(addNewSampleDialog()));
    // Remove selected sample. Slot function will determine which sample is selected.
    connect(removeSampleButton_, SIGNAL(clicked()), this, SLOT(removeSample()));
    // When the user selects a different sample from the list (and thus the model), the QDataWidgetMapper will need to know the new index.
    connect(sampleListView_->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), modelToWidgetMapper_, SLOT(setCurrentModelIndex(QModelIndex)));
    // Updates the sample name within the model as the user changes the name within the widget.
    connect(nameEdit_, SIGNAL(textEdited(QString)), this, SLOT(updateSampleName(QString)));
    // Updates the sample chemical symbol within the model as the user changes the symbol within the widget.
    connect(chemEdit_, SIGNAL(textEdited(QString)), this, SLOT(updateSampleChemSymbol(QString)));
}

void SampleView::setupMainLayout(){

    // Create QListView object and associate it with the sample model.
    sampleListView_ = new QListView();
    sampleListView_->setModel(sampleModel_);
    // Prevent user from editing the text within the view.
    sampleListView_->setEditTriggers(0);
    // Set view to display column 4 (index 3) of the model, the sample title.
    sampleListView_->setModelColumn(3);

    // Sample list and it's groupbox layout setup.
    sampleListGroupBox_ = new QGroupBox("Samples");
    sampleListLayout_ = new QVBoxLayout;
    sampleListLayout_->addWidget(sampleListView_);
    sampleListGroupBox_->setLayout(sampleListLayout_);

    nameLabel_ = new QLabel("Sample Name: ");
    nameEdit_ = new QLineEdit();

    chemLabel_ = new QLabel("Chemical Symbol: ");
    chemEdit_ = new QLineEdit();

    notesLabel_ = new QLabel("Notes: ");
    notesEdit_ = new QTextEdit();

    addSampleButton_ = new QPushButton("Add Sample");
    removeSampleButton_ = new QPushButton("Remove Sample");

    // Setup QDataWidgetMapper. Set approrpite UI elements to their respective rows in sample model.
    // Set the widgets to display the first sample.
    modelToWidgetMapper_ = new QDataWidgetMapper(this);
    modelToWidgetMapper_->setModel(sampleModel_);
    modelToWidgetMapper_->addMapping(nameEdit_, 0);
    modelToWidgetMapper_->addMapping(chemEdit_, 1);
    modelToWidgetMapper_->addMapping(notesEdit_, 2);
    modelToWidgetMapper_->toFirst();

    //Master layout
    sampleWidgetLayout_ = new QHBoxLayout();

    //Sample sub-layout
    sampleNameLayout_ = new QHBoxLayout();
    sampleChemLayout_ = new QHBoxLayout();
    sampleNotesLayout_ = new QHBoxLayout();
    sampleInfoLayout_ = new QVBoxLayout();
    addRemoveButtonLayout_ = new QHBoxLayout();

    //Add widgets to appropriate layouts
    sampleNameLayout_->addWidget(nameLabel_);
    sampleNameLayout_->addWidget(nameEdit_);

    sampleChemLayout_->addWidget(chemLabel_);
    sampleChemLayout_->addWidget(chemEdit_);

    sampleNotesLayout_->addWidget(notesLabel_);
    sampleNotesLayout_->addWidget(notesEdit_);

    addRemoveButtonLayout_->addWidget(addSampleButton_);
    addRemoveButtonLayout_->addWidget(removeSampleButton_);

    sampleInfoLayout_->addLayout(sampleNameLayout_);
    sampleInfoLayout_->addLayout(sampleChemLayout_);
    sampleInfoLayout_->addLayout(sampleNotesLayout_);
    sampleInfoLayout_->addLayout(addRemoveButtonLayout_);

    //Setup master layout
    sampleWidgetLayout_->addLayout(sampleInfoLayout_);
    sampleWidgetLayout_->addSpacing(10);
    sampleWidgetLayout_->addWidget(sampleListGroupBox_);

    setLayout(sampleWidgetLayout_);

}


void SampleView::addNewSampleDialog(){

    // Instantiate dialog if not done already.
    if(!addSampleDialog_){

        addSampleDialog_ = new QDialog(this, Qt::FramelessWindowHint);
        addSampleDialog_->setFixedSize(300, 300);

        setupAddSampleDialog();

        connect(confirmAddSampleButton_, SIGNAL(clicked()), this, SLOT(validateNewSampleInfo()));
        connect(cancelSampleAdd_, SIGNAL(clicked()), addSampleDialog_, SLOT(close()));

        addSampleDialog_->setLayout(addSampleInfoLayout_);
    }

    addSampleDialog_->exec();
}

// Prevent users from adding a sample without a name.
void SampleView::validateNewSampleInfo(){

    // Check to see if user has inputed a sample name.
    if(newNameEdit_->text().isEmpty()){

        QMessageBox msgBox;
        msgBox.setText("Please add sample name.");
        msgBox.exec();

    } else {
        // Send data from edit fields to the model to be added.
        addSampleToModel(newNameEdit_->text(), newChemEdit_->text(), newNotesEdit_->toPlainText());
        //Clear the fields once the sample has been added to the model. This is so when the user
        //opens the addSampleDialog the previously added sample is gone from the fields.
        newNameEdit_->clear();
        newChemEdit_->clear();
        newNotesEdit_->clear();
        // Close dialog window.
        addSampleDialog_->close();
    }
}


void SampleView::addSampleToModel(QString name, QString chem, QString notes){

    // Models are 0-index so by getting the current row count, that number will be the value
    // of the new row index.
    int newSampleIndexRow = sampleModel_->rowCount();

    // Sample display title is a concatenation of the name and chemical symbol and stored in the 4 column (index 3) of the model.
    QString displayTitle = name + " - " + chem;
    QStandardItem *newItemDisplayTitle = new QStandardItem(displayTitle);

    QStandardItem *newItemName = new QStandardItem(name);
    QStandardItem *newItemChemSymbol = new QStandardItem(chem);
    QStandardItem *newItemNotes = new QStandardItem(notes);

    sampleModel_->setItem(newSampleIndexRow, 0, newItemName);
    sampleModel_->setItem(newSampleIndexRow, 1, newItemChemSymbol);
    sampleModel_->setItem(newSampleIndexRow, 2, newItemNotes);
    sampleModel_->setItem(newSampleIndexRow, 3, newItemDisplayTitle);

}


void SampleView::setupAddSampleDialog(){

    newNameLabel_ = new QLabel("Name: ");
    newNameEdit_ = new QLineEdit();

    newChemLabel_ = new QLabel("Chem: ");
    newChemEdit_ = new QLineEdit();

    newNotesLabel_ = new QLabel("Notes: ");
    newNotesEdit_ = new QTextEdit();

    confirmAddSampleButton_ = new QPushButton("Confirm");
    cancelSampleAdd_ = new QPushButton("Cancel");

    addNameLayout_ = new QHBoxLayout;
    addChemLayout_ = new QHBoxLayout;
    addNotesLayout_ = new QHBoxLayout;
    addSampleInfoLayout_ = new QVBoxLayout;
    addButtonLayout_ = new QHBoxLayout;

    addNameLayout_->addWidget(newNameLabel_);
    addNameLayout_->addWidget(newNameEdit_);
    addChemLayout_->addWidget(newChemLabel_);
    addChemLayout_->addWidget(newChemEdit_);
    addNotesLayout_->addWidget(newNotesLabel_);
    addNotesLayout_->addWidget(newNotesEdit_);
    addButtonLayout_->addWidget(confirmAddSampleButton_);
    addButtonLayout_->addWidget(cancelSampleAdd_);

    addSampleInfoLayout_ = new QVBoxLayout;
    addSampleInfoLayout_->addLayout(addNameLayout_);
    addSampleInfoLayout_->addLayout(addChemLayout_);
    addSampleInfoLayout_->addLayout(addNotesLayout_);
    addSampleInfoLayout_->addLayout(addButtonLayout_);


}


void SampleView::updateSampleName(QString name){


    // Grab model index of currently selected sample.
    QModelIndex currentIndex = sampleListView_->selectionModel()->currentIndex();
    // In order to keep the sample chemical symbol constant as the user updates the name, grab the index of the chem symbol value
    // of the currently selected sample and store it as a string. Sample chem symbol is index-1 of model.
    QModelIndex indexOfChem(currentIndex.model()->index(currentIndex.row(), 1, currentIndex.parent()));
    QString currentChemValue_ = (indexOfChem.data(Qt::DisplayRole).toString());
    // The updated sample title.
    QStandardItem *item = new QStandardItem(name + " - " + currentChemValue_);
    // Set item in the model at the selected index row. Recall column 4 (index 3) is the sample title.
    sampleModel_->setItem(currentIndex.row(), 3, item);
    // Ensure proper mapping from edit widgets to model. Behaviour is buggy without it.
    modelToWidgetMapper_->submit();
    // Once a single character has changed the focus is lost on the sampleListView and any additional changes to the edit will not be
    // noticed by the sampleListView and cause the program to crash. This returns focus, and more imporantly the index selection, back to the sampleListView.
    sampleListView_->selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::Select);

}

void SampleView::updateSampleChemSymbol(QString chemSymbol){


    // Grab model index of currently selected sample.
    QModelIndex currentIndex = sampleListView_->selectionModel()->currentIndex();
    // In order to keep the sample name constant as the user updates the chemical symbol, grab the index of the name value
    // of the currently selected sample and store it as a string. Sample name is index-0 of model.
    QModelIndex indexOfName(currentIndex.model()->index(currentIndex.row(), 0, currentIndex.parent()));
    QString currentNameValue_ = (indexOfName.data(Qt::DisplayRole).toString());
    // The updated sample title.
    QStandardItem *item = new QStandardItem(currentNameValue_ + " - " + chemSymbol);
    // Set the model at the selected index row. Recall column 4 (index 3) is the sample title.
    sampleModel_->setItem(currentIndex.row(), 3, item);
    // Ensure proper mapping from edit widgets to model. Behaviour is buggy without it.
    modelToWidgetMapper_->submit();
    // Once a single character has changed the focus is lost on the sampleListView and any additional changes to the edit will not be
    // noticed by the sampleListView and cause program to crash. This returns focus, and more imporantly the index selection, back to the sampleListView.
    sampleListView_->selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::Select);

}


// When a sample is removed, it's info is still inside the fields. This is incase a user
// accidently removes a sample, they can read it by looking at and copying the fields
// into the new sample dialog.
void SampleView::removeSample(){

    QModelIndex currentIndex = sampleListView_->selectionModel()->currentIndex();
    sampleModel_->removeRow(currentIndex.row());

}

// The model is represented by 4 columns and 0 rows (as model is not pre-populated). Column 1 through 3 are the Sample Name,
// it's Chemical Symbol and any Notes needed. The 4th column (index 3) represents the title that is displayed in the QListView sampleListView_
// and is simply a string concatenation of the name and chemical symbol.
void SampleView::setupModel(){

    sampleModel_ = new QStandardItemModel(0, 4, this);

}

// Clean up dialog
SampleView::~SampleView()
{
    if(addSampleDialog_){
        addSampleDialog_->deleteLater();
        addSampleDialog_ = 0;
    }
}
