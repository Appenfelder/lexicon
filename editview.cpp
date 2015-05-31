#include "editview.h"
#include "application.h"
#include "definedsettings.h"
#include "externs.h"
#define EDIT_CSS 0
#define EDIT_XSLT 1
EditPage::EditPage(int type,QWidget * parent) : QWidget(parent) {
  m_type = type;
  readSettings();
  QVBoxLayout * layout = new QVBoxLayout;
  m_text = new QPlainTextEdit;
  if (type == EDIT_CSS) {
    m_useOther = new QCheckBox(tr("Use edited XSLT"));
  }
  else {
    m_useOther = new QCheckBox(tr("Use edited CSS"));
  }

  m_buttons =   new QDialogButtonBox(QDialogButtonBox::Apply|QDialogButtonBox::RestoreDefaults,this);
  connect(m_buttons,SIGNAL(clicked(QAbstractButton *)),this,SLOT(onClicked(QAbstractButton *)));
  connect(m_text,SIGNAL(textChanged()),this,SLOT(onTextChanged()));

  layout->addWidget(m_text);
  layout->addWidget(m_useOther);
  layout->addWidget(m_buttons);
  setLayout(layout);
  this->restore();
  //  m_buttons->button(QDialogButtonBox::Reset)->setEnabled(false);
}
QString EditPage::getText() const {
  return m_text->toPlainText();

}
QString EditPage::getOriginalText() const {
  return m_lines.join("\n");

}
void EditPage::apply() {
  emit(apply(m_type,m_useOther->isChecked()));
}
void EditPage::reset() {
  m_text->setPlainText(m_lines.join("\n"));
  //  m_buttons->button(QDialogButtonBox::Reset)->setEnabled(false);
}
void EditPage::restore() {
  m_lines.clear();
  this->readFile(m_fileName);
  m_text->setPlainText(m_lines.join("\n"));
  //  m_buttons->button(QDialogButtonBox::Reset)->setEnabled(false);
  emit(modified(m_type,false));
}

void EditPage::onClicked(QAbstractButton * btn) {
  QPushButton * button = qobject_cast<QPushButton *>(btn);
  if (! button )
    return;

  if (button == m_buttons->button(QDialogButtonBox::Apply)) {
    this->apply();
    return;
  }
  if (button == m_buttons->button(QDialogButtonBox::RestoreDefaults)) {
    this->restore();
    return;
  }
  if (button == m_buttons->button(QDialogButtonBox::Reset)) {
    this->reset();
    return;
  }
}
void EditPage::onTextChanged() {
  //  m_buttons->button(QDialogButtonBox::Reset)->setEnabled(true);
  emit(modified(m_type,true));
}
void EditPage::readFile(const QString & name) {
  QFile f(name);
  if (! f.open(QIODevice::ReadOnly)) {
    QLOG_WARN()  << QString(tr("Cannot open file %1:  %2")).arg(name).arg(f.errorString());
    return;
  }
  QTextStream in(&f);
  while(! in.atEnd()) {
    m_lines << in.readLine();
  }
  f.close();
}
bool EditPage::writeFile() {
  QFile f(m_fileName);
  if (! f.open(QIODevice::WriteOnly)) {
    QString msg = QString(tr("Cannot open file %1 for writing: %2\n")).arg(m_fileName).arg(qPrintable(f.errorString()));
    QString title;
    if (m_type == EDIT_CSS) {
      title = tr("Edit CSS");
    }
    else {
      title = tr("Edit XSLT");
    }
    QMessageBox::warning(this,
                         title,
                         msg + "\n" + tr("Unable to save changes.\nYou may wish to copy the text and save outside the application."),
                         QMessageBox::Ok);
    return false;
  }
  QTextStream out(&f);
  out << m_text->toPlainText();
  f.close();
  return true;
}

void EditPage::readSettings() {
  SETTINGS
  settings.beginGroup("Entry");
  if (m_type == EDIT_CSS) {
    m_fileName = settings.value(SID_ENTRY_CSS,QString("entry.css")).toString();
    m_fileName = getLexicon()->getResourceFilePath(Lexicon::Stylesheet,m_fileName);
  }
  else {
    m_fileName = settings.value(SID_XSLT_ENTRY,QString("entry.xslt")).toString();
    m_fileName = getLexicon()->getResourceFilePath(Lexicon::XSLT,m_fileName);

  }
  readFile(m_fileName);
}
/**
 * EditView
 *
 * @param parent
 */
EditView::EditView(QWidget * parent) : QWidget(parent) {
  setWindowTitle(tr("CSS/XSLT Editor"));
  readSettings();
  QVBoxLayout * layout = new QVBoxLayout;
  m_tabs = new QTabWidget;
  m_tabs->setObjectName("edittabs");
  m_cssEditor = new EditPage(EDIT_CSS);
  //  m_cssEditor->setText(m_css.join("\n"));
  m_tabs->addTab(m_cssEditor,tr("CSS"));

  m_xsltEditor = new EditPage(EDIT_XSLT);
  //  m_xsltEditor->setText(m_xslt.join("\n"));
  m_tabs->addTab(m_xsltEditor,tr("XSLT"));
  m_buttons = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
  layout->addWidget(m_tabs);
  layout->addWidget(m_buttons);
  connect(m_buttons,SIGNAL(accepted()),this,SLOT(accept()));
  connect(m_buttons,SIGNAL(rejected()),this,SLOT(reject()));

  connect(m_cssEditor,SIGNAL(apply(int,bool)),this,SLOT(apply(int,bool)));
  connect(m_xsltEditor,SIGNAL(apply(int,bool)),this,SLOT(apply(int,bool)));
  connect(m_cssEditor,SIGNAL(modified(int,bool)),this,SLOT(modified(int,bool)));
  connect(m_xsltEditor,SIGNAL(modified(int,bool)),this,SLOT(modified(int,bool)));
  m_cssModified = false;
  m_xsltModified = false;
  m_buttons->button(QDialogButtonBox::Save)->setEnabled(m_cssModified || m_xsltModified);

  setLayout(layout);
}
EditView::~EditView() {
  writeSettings();
}
void EditView::accept() {
  if (m_cssEditor->writeFile() &&
      m_xsltEditor->writeFile()) {
    m_buttons->button(QDialogButtonBox::Save)->setEnabled(false);
    return;
  }

}
void EditView::reject() {
  this->hide();
}
void EditView::modified(int type,bool isDirty) {
  if (type == EDIT_CSS) {
    m_cssModified = isDirty;
  }
  if (type == EDIT_XSLT) {
    m_xsltModified = isDirty;
  }
  m_buttons->button(QDialogButtonBox::Save)->setEnabled(m_cssModified || m_xsltModified);
}


QSize EditView::sizeHint() const {
  return QSize(800,600);
}
void EditView::writeSettings() {
  SETTINGS
  settings.beginGroup("EntryLayout");
  settings.setValue("Size", size());
  settings.setValue("Pos", pos());
  settings.endGroup();
  settings.sync();
}
void EditView::readSettings() {
  SETTINGS

  settings.beginGroup("EditView");
  move(settings.value(SID_EDITVIEW_POS,QPoint(400,150)).toPoint());
  resize(settings.value(SID_EDITVIEW_SIZE,QSize(800,60)).toSize());
  settings.endGroup();
}
void EditView::apply(int type,bool useEdited) {
  QString css;
  QString xslt;
  if (type == EDIT_CSS) {
    css = m_cssEditor->getText();
    if (useEdited) {
      xslt = m_xsltEditor->getText();
    }
    else {
      xslt = m_xsltEditor->getOriginalText();
    }
  }
  if (type == EDIT_XSLT) {
    xslt = m_xsltEditor->getText();
    if (useEdited) {
      css = m_cssEditor->getText();
    }
    else {
      css = m_cssEditor->getOriginalText();
    }
  }
  if (! css.isEmpty() && ! xslt.isEmpty()) {
    emit(reload(css,xslt));
  }
}
