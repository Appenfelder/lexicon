#include "editview.h"
#include "application.h"
#include "definedsettings.h"
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

  m_buttons =   new QDialogButtonBox(QDialogButtonBox::Apply|QDialogButtonBox::Reset|QDialogButtonBox::RestoreDefaults,this);
  connect(m_buttons,SIGNAL(clicked(QAbstractButton *)),this,SLOT(onClicked(QAbstractButton *)));
  layout->addWidget(m_text);
  layout->addWidget(m_useOther);
  layout->addWidget(m_buttons);
  setLayout(layout);
  m_text->setPlainText(m_lines.join("\n"));
}
QString EditPage::getText() const {
  return m_text->toPlainText();

}
void EditPage::apply() {

}
void EditPage::reset() {

}
void EditPage::restore() {
  qDebug() << Q_FUNC_INFO;
  m_lines.clear();
  this->readFile(m_fileName);
  m_text->setPlainText(m_lines.join("\n"));
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

void EditPage::readSettings() {
  QString fileName;
  QScopedPointer<QSettings> settings((qobject_cast<Lexicon *>(qApp))->getSettings());

  settings->beginGroup("Entry");
  if (m_type == EDIT_CSS) {
    m_fileName = settings->value(SID_ENTRY_CSS,QString("entry.css")).toString();
  }
  else {
    m_fileName = settings->value(SID_XSLT_ENTRY,QString("entry.xslt")).toString();
  }
  readFile(m_fileName);
}

EditView::EditView(QWidget * parent) : QWidget(parent) {
  readSettings();
  QVBoxLayout * layout = new QVBoxLayout;
  m_tabs = new QTabWidget;

  m_cssEditor = new EditPage(EDIT_CSS,this);
  //  m_cssEditor->setText(m_css.join("\n"));
  m_tabs->addTab(m_cssEditor,tr("CSS"));

  m_xsltEditor = new EditPage(EDIT_XSLT,this);
  //  m_xsltEditor->setText(m_xslt.join("\n"));
  m_tabs->addTab(m_xsltEditor,tr("XSLT"));
  m_buttons = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
  layout->addWidget(m_tabs);
  layout->addWidget(m_buttons);
  connect(m_buttons,SIGNAL(accepted()),this,SLOT(accept()));
  connect(m_buttons,SIGNAL(rejected()),this,SLOT(reject()));
  setLayout(layout);
}
void EditView::accept() {
  qDebug() << Q_FUNC_INFO;
  qDebug() << m_cssEditor->getText();
}
void EditView::reject() {
  qDebug() << Q_FUNC_INFO;
  m_cssEditor->restore();
  m_xsltEditor->restore();
  this->hide();
}



QSize EditView::sizeHint() const {
  return QSize(800,600);
}
void EditView::readSettings() {
  Lexicon * app = qobject_cast<Lexicon *>(qApp);
  QScopedPointer<QSettings> settings((qobject_cast<Lexicon *>(qApp))->getSettings());
  settings->beginGroup("EntryLayout");
  this->restoreGeometry(settings->value("Geometry").toByteArray());
  settings->endGroup();
}
