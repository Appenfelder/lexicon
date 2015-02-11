#include "rootsoptions.h"
#include "definedsettings.h"
#include "QsLog.h"
#ifndef STANDALONE
#include "application.h"
#endif
RootsOptions::RootsOptions(const QString & theme,QWidget * parent) : OptionsWidget(theme,parent) {
  /*
#ifdef STANDALONE
  m_settings = new QSettings("default.ini",QSettings::IniFormat);
  m_settings->setIniCodec("UTF-8");
#else
  m_settings = (qobject_cast<Lexicon *>(qApp))->getSettings();
#endif
  */
  m_section = "Roots";

  QVBoxLayout * vlayout = new QVBoxLayout;
  QFormLayout * formlayout = new QFormLayout;

  m_moveDown = new QLineEdit;
  m_moveDown->setMaxLength(1);
  m_moveUp = new QLineEdit;
  m_moveUp->setMaxLength(1);
  m_expand = new QLineEdit;
  m_expand->setMaxLength(1);

  m_singleClick = new QCheckBox;
  m_doubleClick = new QCheckBox;
  m_debug = new QCheckBox;

  formlayout->addRow(tr("Move up"),m_moveUp);
  formlayout->addRow(tr("Move down"),m_moveDown);
  formlayout->addRow(tr("Expand/collapse"),m_expand);
  formlayout->addRow(tr("Single click activates item"),m_singleClick);
  formlayout->addRow(tr("Double click activates item"),m_doubleClick);

  QHBoxLayout * hlayout1 = new QHBoxLayout;
  m_standardFont = new QLineEdit;
  m_standardButton = new QPushButton(tr("Click to set font"));
  hlayout1->addWidget(m_standardFont);
  hlayout1->addWidget(m_standardButton);
  formlayout->addRow(tr("Non-Arabic font"),hlayout1);

  QHBoxLayout * hlayout2 = new QHBoxLayout;
  m_arabicFont = new QLineEdit;
  m_arabicButton = new QPushButton(tr("Click to set font"));
  hlayout2->addWidget(m_arabicFont);
  hlayout2->addWidget(m_arabicButton);
  formlayout->addRow(tr("Arabic font"),hlayout2);

  connect(m_standardButton,SIGNAL(clicked()),this,SLOT(onSetFont()));
  connect(m_arabicButton,SIGNAL(clicked()),this,SLOT(onSetFont()));


  formlayout->addRow(tr("Enable debug"),m_debug);

  vlayout->addLayout(formlayout);

  m_info = new QLabel;

  QStringList html;
  html << "<h4>";
  html << tr("Notes:");
  html << "</h4>";
  html << "<ul>";
  html << "<li>";
  html << tr("Up/down arrows  can be used to move around the tree");
  html << "</li>";
  html << "<li>";
  html << tr("Space will expand/collapse the current node");
  html << "</li>";
  html << "<li>";
  html << tr("Return on a head word will activate the item");
  html << "</li>";
  html << "</ul>";
  m_info->setText(html.join(""));
  vlayout->addWidget(m_info);
  setLayout(vlayout);
  addButtons();
  readSettings();
  setupConnections();
}
void RootsOptions::readSettings() {
  qDebug() << Q_FUNC_INFO << m_settingsFileName;
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  m_debug->setChecked(settings.value(SID_CONTENTS_DEBUG).toBool());
  m_moveUp->setText(settings.value(SID_CONTENTS_MOVE_UP).toString());
  m_moveDown->setText(settings.value(SID_CONTENTS_MOVE_DOWN).toString());
  m_expand->setText(settings.value(SID_CONTENTS_EXPAND).toString());

  m_arabicFont->setText(settings.value(SID_CONTENTS_ARABIC_FONT).toString());
  m_standardFont->setText(settings.value(SID_CONTENTS_STANDARD_FONT).toString());
  settings.endGroup();
  m_dirty = false;
}
void RootsOptions::writeSettings() {
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);
  settings.setValue(SID_CONTENTS_DEBUG,m_debug->isChecked());
  settings.setValue(SID_CONTENTS_MOVE_UP,m_moveUp->text());
  settings.setValue(SID_CONTENTS_MOVE_DOWN,m_moveDown->text());
  settings.setValue(SID_CONTENTS_EXPAND,m_expand->text());
  settings.setValue(SID_CONTENTS_ARABIC_FONT,m_arabicFont->text());
  settings.setValue(SID_CONTENTS_STANDARD_FONT,m_standardFont->text());
  settings.sync();

  settings.endGroup();
}
/**
 * TODO not complete
 *
 *
 * @return
 */
bool RootsOptions::isModified()  {
  bool v;
  QString s;
  m_dirty = false;
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  v = settings.value(SID_CONTENTS_DEBUG).toBool();
  if (m_debug->isChecked() != v) {
    m_dirty = true;
  }
  s = settings.value(SID_CONTENTS_MOVE_UP).toString();
  if (m_moveUp->text() != s) {
    m_dirty = true;
  }
  s = settings.value(SID_CONTENTS_MOVE_DOWN).toString();
  if (m_moveDown->text() != s) {
    m_dirty = true;
  }
  settings.endGroup();
  qDebug() << Q_FUNC_INFO << m_dirty;
  return m_dirty;
}
void RootsOptions::onSetFont() {
  QPushButton * button = qobject_cast<QPushButton *>(QObject::sender());
  QFont f;
  if (button == m_standardButton) {
    f.fromString(m_standardFont->text());
  }
  else {
    f.fromString(m_arabicFont->text());
  }
  bool ok;
  QFont font = QFontDialog::getFont(&ok, f,  this);
  if (!ok) {
    return;
  }
  if (button == m_standardButton) {
    m_standardFont->setText(font.toString());
  }
  else {
    m_arabicFont->setText(font.toString());
  }
}
