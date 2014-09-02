#include "notedialog.h"
#include "notes.h"
#ifndef TEST_FRAMEWORK
#include "laneslexicon.h"
#endif
extern NoteMaster * getNotes();
/**
 * This constructor is used when adding a new note
 *
 * @param p
 * @param parent
 */
NoteDialog::NoteDialog(const Place & p,QWidget * parent) : QDialog(parent) {
  m_id = -1;
  m_place = p;
  m_word = p.getWord();
  this->setup();
  setWindowTitle(m_subject->text());
  setObjectName("notedialog");
  /// set values from place
  m_subject->setText(p.getShortText());
  m_note->setFocus();
}
/**
 * This constructor is used when updating a note
 *
 * @param note
 * @param parent
 */
NoteDialog::NoteDialog(Note * note ,QWidget * parent) : QDialog(parent) {
  m_id = note->getId();
  m_place = note->getPlace();
  m_word = note->getWord();
  m_subjectText = note->getSubject();
  m_noteText = note->getNote();
  m_noteType = note->getType();

  this->setup();
  /// set values from note
  m_subject->setText(m_subjectText);
  m_note->setText(m_noteText);
  m_type->setCurrentIndex(m_type->findData(m_noteType));
  m_note->setFocus();
  setWindowTitle(m_subject->text());
}
void NoteDialog::setup() {
  QFormLayout * layout = new QFormLayout;
  m_subject = new QLineEdit(this);

  m_note = new QTextEdit(this);


  m_moreButton = new QPushButton(tr("&More"));
  m_moreButton->setCheckable(true);
  m_moreButton->setAutoDefault(false);

  m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel,Qt::Horizontal);
  //  m_buttonBox->addButton(new QPushButton("&Save"),QDialogButtonBox::AcceptRole);
  //  m_buttonBox->addButton(new QPushButton("&Cancel"),QDialogButtonBox::RejectRole);
  m_buttonBox->addButton(m_moreButton, QDialogButtonBox::ActionRole);

  m_printButton = new QPushButton(tr("&Print"));
  m_keyboardButton  = new QPushButton(tr("Show &keyboard"));
  m_keyboardButton->setAutoDefault(false);

  m_moreButtonBox = new QDialogButtonBox(Qt::Horizontal);
  m_moreButtonBox->addButton(m_keyboardButton, QDialogButtonBox::ActionRole);
  m_moreButtonBox->addButton(m_printButton, QDialogButtonBox::ActionRole);

  m_moreButtonBox->hide();
  connect(m_moreButton, SIGNAL(toggled(bool)), this, SLOT(showOptions(bool)));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(save()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
  connect(m_keyboardButton, SIGNAL(clicked()),this,SLOT(showKeyboard()));
  connect(m_printButton,SIGNAL(clicked()),this,SLOT(print()));

  m_type = new QComboBox;
  m_type->insertItem(0,tr("User"),Note::User);
  m_type->insertItem(1,tr("System"),Note::Error);
  m_type->setCurrentIndex(0);
  connect(m_type,SIGNAL(onCurrentIndexChanged(int)),this,SLOT(onTypeChange(int)));
  layout->addRow(tr("Subject"),m_subject);
  layout->addRow(tr("Note"),m_note);
  layout->addRow(tr("Type"),m_type);
  layout->addRow(m_buttonBox);
  layout->addRow(m_moreButtonBox);
  //  layout->addWidget(m_moreButtonBox);
  setLayout(layout);
  //  setWindowFlags(Qt::CustomizeWindowHint);
  setSizeGripEnabled(true);
  m_changed = false;
  m_autosave = false;
  setWindowTitle(m_subject->text());
  m_attached = false;
  m_keyboard = new KeyboardWidget(this);
  QPoint p = this->pos();

  int h = this->frameGeometry().height();

  m_keyboard->move(p.x(),p.y() + h);
}
void NoteDialog::showOptions(bool v) {
  if (v)
    m_moreButtonBox->show();
  else
    m_moreButtonBox->hide();
}
void NoteDialog::setModified(bool v) {
  m_changed = v;
  m_note->document()->setModified(v);
}
QSize NoteDialog::sizeHint() const {
  QSize sz(300,300);
  return sz;
}
void NoteDialog::closeEvent(QCloseEvent * event) {
  QLOG_DEBUG() << Q_FUNC_INFO << "modified" << this->isModified();
  if (this->isModified()) {
    if (m_autosave) {
      this->save();
    }
    else {
      QMessageBox msgBox;
      msgBox.setText("The note has been modified.");
      msgBox.setInformativeText("Do you want to save your changes?");
      msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      msgBox.setDefaultButton(QMessageBox::Save);
      msgBox.setWindowModality(Qt::ApplicationModal);
      int ret = msgBox.exec();
      if (ret == QMessageBox::Save) {
        this->save();
      }
    }
  }
  QDialog::closeEvent(event);
}
NoteDialog::~NoteDialog() {
  QLOG_DEBUG() << Q_FUNC_INFO;

}
void NoteDialog::setSubject(const QString & text) {
  m_subject->setText(text);
  //  m_data.setSubject(text);
  m_changed = true;
}
bool NoteDialog::isModified() const {
  if (m_changed || m_note->document()->isModified())
    return true;

  return false;
}
void NoteDialog::showKeyboard() {
  m_keyboard->attach(m_note);
  m_attached = ! m_attached;
  if (m_attached) {
    m_keyboardButton->setText(tr("Hide keyboard"));
    QPoint p;
    p = m_keyboard->currentPosition();
    if (p.isNull()) {
      p = this->pos();
      int h = this->frameGeometry().height();
    /// TODO adjust this
      m_keyboard->move(p.x() - 50,p.y() + h);
    }
    else {
      m_keyboard->move(p);
    }
  }
  else
    m_keyboardButton->setText(tr("Show keyboard"));

}
void NoteDialog::cancel() {
  m_note->setText(m_noteText);
  m_subject->setText(m_subjectText);
  m_changed = false;
  if (m_attached)
    showKeyboard();

  this->reject();
}
void NoteDialog::save() {
  qDebug() << Q_FUNC_INFO;
  Note * n = new Note();
  n->setPlace(m_place);
  n->setId(m_id);
  n->setWord(m_word);
  n->setSubject(m_subject->text());
  n->setNote(m_note->toPlainText());
  n->setType(m_type->currentData().toInt());
  if (m_attached) {
    showKeyboard();
  }
  NoteMaster * notes = ::getNotes();
  bool ok = notes->save(n);
  delete n;
  if (ok) {
    m_changed = false;
    m_note->document()->setModified(false);
    m_subjectText = m_subject->text();
    m_noteText = m_note->toPlainText();
    m_noteType =  m_type->currentData().toInt();

    emit(noteSaved(ok));
  }
  this->accept();
}
void NoteDialog::print() {
  QPrinter printer;
  QPrintDialog printDialog(&printer, this);
  if (printDialog.exec() == QDialog::Accepted) {
    // print ...

    //  printer->setPaperSize(QPrinter::A4);
    //  printer->setOutputFileName();
    //    QPainter painter(&m_printer);
    //    painter.setRenderHint(QPainter::Antialiasing);
    m_note->print(&printer);
  }
}
void NoteDialog::onTypeChange(int ix) {
  if (ix == -1)
    return;

  int type = m_type->currentData().toInt();
  if (type != m_noteType) {
    m_changed = true;
  }
}
