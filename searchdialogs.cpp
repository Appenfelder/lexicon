#include "searchdialogs.h"
#include "QsLog.h"
#include "namespace.h"
#include "searchoptions.h"
ArabicSearchDialog::ArabicSearchDialog(int searchType,QWidget * parent,Qt::WindowFlags f) :
  QDialog(parent,f) {

  switch(searchType) {
    case Lane::Root :
    setWindowTitle(tr("Search for Root"));
    break;
  case Lane::Entry :
    setWindowTitle(tr("Search for Entry"));
    break;
  case Lane::Word :
    setWindowTitle(tr("Search for Word"));
    break;
  default :break;
  }



  m_prompt = new QLabel(tr("Find"));
  m_edit = new ImLineEdit;
  Lexicon * app = qobject_cast<Lexicon *>(qApp);
  QSettings * settings = app->getSettings();
  m_edit->readSettings(settings);
  m_prompt->setBuddy(m_edit);
  m_findButton = new QPushButton(tr("&Find"));
  m_findButton->setDefault(true);

  m_keyboardButton  = new QPushButton(tr("Show &keyboard"));
  m_keyboardButton->setAutoDefault(false);

  m_moreButton = new QPushButton(tr("&More"));
  m_moreButton->setCheckable(true);
  m_moreButton->setAutoDefault(false);



  m_buttonBox = new QDialogButtonBox(Qt::Vertical);
  m_buttonBox->addButton(m_findButton, QDialogButtonBox::AcceptRole);
  m_buttonBox->addButton(new QPushButton("&Cancel"),QDialogButtonBox::RejectRole);
  m_buttonBox->addButton(m_moreButton, QDialogButtonBox::ActionRole);
  m_buttonBox->addButton(m_keyboardButton,QDialogButtonBox::ActionRole);

  m_findButton->setFocus();

  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_keyboardButton, SIGNAL(clicked()),this,SLOT(showKeyboard()));

  m_options = new SearchOptions(searchType);

  connect(m_options,SIGNAL(loadKeymap(const QString &)),this,SLOT(loadKeymap(const QString &)));

  QStringList maps = m_edit->getMaps();
  QString map = m_edit->getActiveMap();
  qDebug() << Q_FUNC_INFO << "active map" << map;
  if (map.isEmpty()) {
    map = m_edit->getNullMap();
  }
  maps << m_edit->getNullMap();
  m_options->addKeymaps(map,maps);

  connect(m_moreButton, SIGNAL(toggled(bool)), this, SLOT(showOptions(bool)));


  QVBoxLayout * leftlayout = new QVBoxLayout;

  QHBoxLayout * inputlayout = new QHBoxLayout;
  inputlayout->addWidget(m_prompt);
  inputlayout->addWidget(m_edit);

  leftlayout->addLayout(inputlayout);
  leftlayout->addWidget(m_options);
  leftlayout->addStretch();

  QHBoxLayout * hlayout = new QHBoxLayout;
  hlayout->addLayout(leftlayout);
  hlayout->addStretch();
  hlayout->addWidget(m_buttonBox);

  QVBoxLayout * mainlayout = new QVBoxLayout;
  mainlayout->addLayout(hlayout);
  //  mainlayout->addStretch();
  setLayout(mainlayout);



  m_options->showMore(false);

  m_edit->setFocus();
  m_attached = false;
  m_keyboard = new KeyboardWidget(this);
  QPoint p = this->pos();
  //  QLOG_DEBUG() << "Search dialog pos" << this->pos() << "mapped to global" <<  this->mapToGlobal(this->pos());
  int h = this->frameGeometry().height();
  //  QLOG_DEBUG() << "search dialog frame geometry" << this->frameGeometry();
  m_keyboard->move(p.x(),p.y() + h);
  //  delete settings;
}
void ArabicSearchDialog::showKeyboard() {
  m_keyboard->attach(m_edit);
  m_attached = ! m_attached;
  if (m_attached) {
    m_keyboardButton->setText(tr("Hide keyboard"));
    QPoint p = this->pos();
    int h = this->frameGeometry().height();
    /// TODO adjust this
    m_keyboard->move(p.x() - 50,p.y() + h);
  }
  else
    m_keyboardButton->setText(tr("Show keyboard"));

}
void ArabicSearchDialog::showOptions(bool v) {
  m_options->showMore(v);
  if (!v)
    m_edit->setFocus();

  /// this shrinks the dialog
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}
QString ArabicSearchDialog::getText() {
  return m_edit->text().trimmed();
}
void ArabicSearchDialog::setPrompt(const QString & text) {
  m_prompt->setText(text);
}
void ArabicSearchDialog::setOptions(int opts) {
  m_options->setOptions(opts);
}
int ArabicSearchDialog::getOptions() {
  return m_options->getOptions();
}
void ArabicSearchDialog::loadKeymap(const QString & mapname) {
  m_edit->activateMap(mapname,true);
}
/**
 *
 *
 * @param parent
 * @param f
 */
NodeSearchDialog::NodeSearchDialog(QWidget * parent,Qt::WindowFlags f) :
  QDialog(parent,f) {
  setWindowTitle(tr("Search for node"));

  m_prompt = new QLabel(tr("Find &node"));
  m_edit = new QLineEdit;
  m_prompt->setBuddy(m_edit);

  m_newTab = new QCheckBox(tr("Open in &new tab"));
  m_switchFocus = new QCheckBox(tr("Switch to &new tab"));


  m_findButton = new QPushButton(tr("&Find"));
  m_findButton->setDefault(true);


  m_options = new QWidget;

  m_buttonBox = new QDialogButtonBox(Qt::Vertical);
  m_buttonBox->addButton(m_findButton, QDialogButtonBox::AcceptRole);
  m_buttonBox->addButton(new QPushButton("&Cancel"),QDialogButtonBox::RejectRole);

  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  QVBoxLayout * optionsLayout = new QVBoxLayout;
  optionsLayout->setMargin(0);
  optionsLayout->addWidget(m_switchFocus);
  m_options->setLayout(optionsLayout);

  //  connect(m_moreButton, SIGNAL(toggled(bool)), this, SLOT(showOptions(bool)));
  QHBoxLayout *topLeftLayout = new QHBoxLayout;
  topLeftLayout->addWidget(m_prompt);
  topLeftLayout->addWidget(m_edit);

  QVBoxLayout *leftLayout = new QVBoxLayout;
  leftLayout->addLayout(topLeftLayout);
  leftLayout->addWidget(m_newTab);

  QGridLayout *mainLayout = new QGridLayout;
  mainLayout->setSizeConstraint(QLayout::SetFixedSize);
  mainLayout->addLayout(leftLayout, 0, 0);
  mainLayout->addWidget(m_buttonBox, 0, 1);
  mainLayout->addWidget(m_options, 1, 0, 1, 2);
  mainLayout->setRowStretch(2, 1);

  setLayout(mainLayout);
}
void NodeSearchDialog::setNewTab(bool v) {
  m_newTab->setChecked(v);
}
QString NodeSearchDialog::getText() const {
  QString t = m_edit->text();
  if (! t.startsWith("n")) {
    t = "n" + t;
  }
  return t;
}
bool NodeSearchDialog::getNewTab() const {
  if (m_newTab->checkState() == Qt::Checked) {
    return true;
  }
  return false;
}
int NodeSearchDialog::getOptions() {
  int options = 0;

  if (m_newTab->checkState() == Qt::Checked)
    options |= Lane::Create_Tab;

  if (m_switchFocus->checkState() == Qt::Checked)
    options |= Lane::Switch_Tab;

  return options;
}
void NodeSearchDialog::setOptions(int options) {
  m_newTab->setChecked(options & Lane::Create_Tab);
  m_switchFocus->setChecked(options & Lane::Switch_Tab);
}
