#include "systemoptions.h"
#include "definedsettings.h"
#include "QsLog.h"
#ifndef STANDALONE
#include "application.h"
#include "externs.h"
#endif
/**
 *
 * @param theme
 * @param parent
 */
SystemOptions::SystemOptions(const QString & theme,QWidget * parent) : OptionsWidget(theme,parent) {
  m_section = "System";
  QVBoxLayout * vlayout = new QVBoxLayout;



  m_contentsLinked = new QCheckBox;
  m_lexicon = new QLineEdit;
  m_debug = new QCheckBox;
  m_docked = new QCheckBox;
  m_focusTab = new QLineEdit;
  m_minimalInterface = new QCheckBox;
  m_restoreBookmarks = new QCheckBox;
  m_restoreTabs = new QCheckBox;
  m_saveSettings = new QCheckBox;
  m_saveTabs = new QCheckBox;
  m_rootNavigation = new QCheckBox;
  // Root mod
  m_runDate = new QDateTimeEdit;
  // Save boo
  m_showInterfaceWarning = new QCheckBox;
  m_css = new QLineEdit;
  m_theme = new QComboBox;
  m_title = new QLineEdit;
  m_toolbarText = new QCheckBox;
  m_useNotes = new QCheckBox;
  m_notesDb = new QLineEdit;
  m_keyboard = new QComboBox;
  QFormLayout * layout = new QFormLayout;

  layout->addRow(tr("Contents linked"),m_contentsLinked);
  layout->addRow(tr("Database"),m_lexicon);
  layout->addRow(tr("Debug"),m_debug);
  layout->addRow(tr("Docked"),m_docked);
  layout->addRow(tr("Current tab"),m_focusTab);
  layout->addRow(tr("Minimal interface"),m_minimalInterface);
  layout->addRow(tr("Restore bookmarks"),m_restoreBookmarks);
  layout->addRow(tr("Restore tab"),m_restoreTabs);
  layout->addRow(tr("Save settings"),m_saveSettings);
  layout->addRow(tr("Run date"),m_runDate);
  layout->addRow(tr("Show interface warning"),m_showInterfaceWarning);
  layout->addRow(tr("Application stylesheet"),m_css);
  layout->addRow(tr("Theme"),m_theme);
  layout->addRow(tr("Title"),m_title);
  layout->addRow(tr("Toolbar text"),m_toolbarText);
  layout->addRow(tr("Nav by root"),m_rootNavigation);
  layout->addRow(tr("Use notes"),m_useNotes);
  layout->addRow(tr("Notes db"),m_notesDb);
  layout->addRow(tr("Keyboard"),m_keyboard);
  vlayout->addLayout(layout);

  vlayout->addStretch();
  setLayout(vlayout);
  addButtons();
  readSettings();
  setupConnections();

}

void SystemOptions::readSettings() {
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  m_contentsLinked->setChecked(settings.value(SID_SYSTEM_CONTENTS_LINKED,true).toBool());
  m_lexicon->setText(settings.value(SID_SYSTEM_DATABASE,"lexicon.sqlite").toString());
  m_debug->setChecked(settings.value(SID_SYSTEM_DEBUG,true).toBool());
  m_docked->setChecked(settings.value(SID_SYSTEM_DOCKED,true).toBool());
  m_focusTab->setText(settings.value(SID_SYSTEM_CURRENT_TAB,"0").toString());
  m_minimalInterface->setChecked(settings.value(SID_SYSTEM_MINIMAL,true).toBool());
  m_restoreBookmarks->setChecked(settings.value(SID_SYSTEM_RESTORE_BOOKMARKS,true).toBool());
  m_restoreTabs->setChecked(settings.value(SID_SYSTEM_RESTORE_TABS,true).toBool());
  m_saveSettings->setChecked(settings.value(SID_SYSTEM_SAVE_SETTINGS,true).toBool());
  m_saveTabs->setChecked(settings.value(SID_SYSTEM_SAVE_TABS,true).toBool());
  m_rootNavigation->setChecked(settings.value(SID_SYSTEM_BY_ROOT,true).toBool());


#ifndef STANDALONE
  m_keyboard->addItems(getLexicon()->getKeyboards());
  m_keyboard->setCurrentText(getLexicon()->getDefaultKeyboard());
  QStringList themes = getLexicon()->getThemes();
  themes.sort();
  m_theme->addItems(themes);
  m_theme->setCurrentText(getLexicon()->currentTheme());
#endif
  QString d = settings.value(SID_SYSTEM_RUN_DATE,QString()).toString();
  m_runDate->setDateTime(QDateTime::fromString(d,Qt::ISODate));

  m_showInterfaceWarning->setChecked(settings.value(SID_SYSTEM_INTERFACE_WARNING,true).toBool());
  m_css->setText(settings.value(SID_SYSTEM_STYLESHEET,"app.css").toString());

  m_title->setText(settings.value(SID_SYSTEM_TITLE,"Lane's Lexicon").toString());
  m_toolbarText->setChecked(settings.value(SID_SYSTEM_TOOLBAR_TEXT,true).toBool());

  settings.endGroup();
  settings.beginGroup("Notes");
  m_useNotes->setChecked(settings.value(SID_NOTES_ENABLED,false).toBool());
  m_notesDb->setText(settings.value(SID_NOTES_DATABASE,"notes.sqlite").toString());

  m_dirty = false;
}
void SystemOptions::writeSettings(const QString & fileName) {
  QString f = m_settingsFileName;
  if (!fileName.isEmpty()) {
    f = fileName;
  }
  qDebug() << Q_FUNC_INFO << f;

  QSettings settings(f,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  settings.setValue(SID_SYSTEM_CONTENTS_LINKED,  m_contentsLinked->isChecked());
  settings.setValue(SID_SYSTEM_DATABASE,m_lexicon->text());
  settings.setValue(SID_SYSTEM_DEBUG,m_debug->isChecked());
  settings.setValue(SID_SYSTEM_DOCKED,m_docked->isChecked());
  settings.setValue(SID_SYSTEM_CURRENT_TAB,m_focusTab->text());
  settings.setValue(SID_SYSTEM_MINIMAL,m_minimalInterface->isChecked());
  settings.setValue(SID_SYSTEM_RESTORE_BOOKMARKS,m_restoreBookmarks->isChecked());
  settings.setValue(SID_SYSTEM_RESTORE_TABS,m_restoreTabs->isChecked());
  settings.setValue(SID_SYSTEM_SAVE_SETTINGS,m_saveSettings->isChecked());
  settings.setValue(SID_SYSTEM_SAVE_TABS,m_saveTabs->isChecked());
  settings.setValue(SID_SYSTEM_BY_ROOT,m_rootNavigation->isChecked());

  settings.setValue(SID_SYSTEM_RUN_DATE,m_runDate->dateTime().toString(Qt::ISODate));

  settings.setValue(SID_SYSTEM_INTERFACE_WARNING,m_showInterfaceWarning->isChecked());
  settings.setValue(SID_SYSTEM_STYLESHEET,m_css->text());

  settings.setValue(SID_SYSTEM_TITLE,m_title->text());
  settings.setValue(SID_SYSTEM_TOOLBAR_TEXT,m_toolbarText->isChecked());

  settings.endGroup();
  settings.beginGroup("Notes");
  settings.setValue(SID_NOTES_ENABLED,m_useNotes->isChecked());
  settings.setValue(SID_NOTES_DATABASE,m_notesDb->text());
#ifndef STANDALONE
  getLexicon()->setTheme(m_theme->currentText());
  getLexicon()->setDefaultKeyboard(m_keyboard->currentText());
#else

#endif
  m_dirty = false;
  emit(modified(false));
}
/**
 * TODO not complete
 *
 *
 * @return
 */
bool SystemOptions::isModified()  {
  m_dirty = false;
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  if (compare(&settings,SID_SYSTEM_CONTENTS_LINKED,m_contentsLinked)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_DATABASE,m_lexicon)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_DEBUG,m_debug)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_DOCKED,m_docked)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_CURRENT_TAB,m_focusTab)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_MINIMAL,m_minimalInterface)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_RESTORE_BOOKMARKS,m_restoreBookmarks)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_RESTORE_TABS,m_restoreTabs)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_SAVE_SETTINGS,m_saveSettings)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_SAVE_TABS,m_saveTabs)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_BY_ROOT,m_rootNavigation)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_RUN_DATE,m_runDate)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_INTERFACE_WARNING,m_showInterfaceWarning)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_STYLESHEET,m_css)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_SYSTEM_TITLE,m_title)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_SYSTEM_TOOLBAR_TEXT,m_toolbarText)) {
    m_dirty = true;
  }

  settings.endGroup();
  settings.beginGroup("Notes");
  if (compare(&settings,SID_NOTES_ENABLED,m_useNotes)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_NOTES_DATABASE,m_notesDb)) {
    m_dirty = true;
  }
#ifndef STANDALONE
  if (m_keyboard->currentText() !=   getLexicon()->getDefaultKeyboard()) {
    m_dirty = true;
  }
  if (m_theme->currentText() != getLexicon()->currentTheme()) {
    m_dirty = true;
  }

#endif
  return m_dirty;
}
void SystemOptions::onSetFont() {
  /*
  QFont f;
  f.fromString(m_font->text());

  QFontDialog * d = new QFontDialog(f);
  QList<QComboBox *> boxes = d->findChildren<QComboBox *>();
  if (boxes.size() == 1) {
    int ix = boxes[0]->findText("Arabic");
    if (ix >= 0) {
      boxes[0]->setCurrentText("Arabic");
      boxes[0]->activated(ix);
    }
  }
  d->setCurrentFont(f);
  if (d->exec() == QDialog::Accepted) {
    QFont font = d->currentFont();
    m_font->setText(font.toString());
  }
  delete d;
  return;
  */
}
void SystemOptions::onSetColor() {
  /*
  QColor color;
  color.setNamedColor(m_highlightColor->text());
  QColorDialog d(color);
  if (d.exec() != QDialog::Accepted) {
    return;
  }
  int r,g,b;
  color = d.currentColor();
  color.getRgb(&r,&g,&b);
  QString str = QString("%1,%2,%3").arg(r).arg(g).arg(b);
  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup("Colors");
  QStringList keys = settings.allKeys();
  QStringList v;
  for(int i=0;i < keys.size();i++) {
    v = settings.value(keys[i]).toStringList();
    if (v.join(",") == str) {

      m_highlightColor->setText(keys[i]);
      return;
    }
  }
  m_highlightColor->setText(d.currentColor().name());
  */
}
