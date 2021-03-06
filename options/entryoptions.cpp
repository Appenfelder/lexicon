#include "entryoptions.h"
#include "definedsettings.h"
#include "QsLog.h"
#ifndef STANDALONE
#include "application.h"
#include "externs.h"
#endif
DebugOptionsDialog::DebugOptionsDialog(bool xml,bool html,bool ohtml,const QString & path,QWidget * parent) : QDialog(parent) {
  QVBoxLayout * layout = new QVBoxLayout;

  QFormLayout * formlayout = new QFormLayout;
  setWindowTitle(tr("Advanced Options"));
  m_xml = new QCheckBox;
  m_html = new QCheckBox;
  m_outputHtml = new QCheckBox;
  m_path = new QLineEdit;

  m_xml->setChecked(xml);
  m_html->setChecked(html);
  m_outputHtml->setChecked(ohtml);
  m_path->setText(path);
  formlayout->addRow(new QLabel(tr("Save generated XML/HTML")));
  formlayout->addRow(tr("XML"),m_xml);
  formlayout->addRow(tr("XSLT output HTML"),m_outputHtml);
  formlayout->addRow(tr("Qt HTML"),m_html);

  QHBoxLayout * hlayout = new QHBoxLayout;
  QPushButton * pathButton = new QPushButton(tr("Set path"));
  connect(pathButton,SIGNAL(clicked()),this,SLOT(onPathSelect()));
  hlayout->addWidget(m_path);
  hlayout->addWidget(pathButton);
  hlayout->addStretch();

  formlayout->addRow(tr("Output directory"),hlayout);


  layout->addLayout(formlayout);

  QDialogButtonBox * btns = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

  layout->addWidget(btns);

  connect(btns,SIGNAL(accepted()),this,SLOT(accept()));
  connect(btns,SIGNAL(rejected()),this,SLOT(reject()));

  setLayout(layout);
}
void DebugOptionsDialog::onPathSelect() {
  QString d  = QFileDialog::getExistingDirectory(this, QString(tr("Select directory")), m_path->text());
  if (! d.isEmpty()) {
    m_path->setText(d);
  }
}
/**
 * Not done:
 *
 * @param theme
 * @param parent
 */
EntryOptions::EntryOptions(const QString & theme,QWidget * parent) : OptionsWidget(theme,parent) {
  setObjectName("entryoptions");
  m_section = "Entry";
  m_nodeinfoClose = new QCheckBox;
  m_back = new QLineEdit ;
  m_css = new QLineEdit ;
  m_printCss = new QLineEdit ;
  m_entryXslt = new QLineEdit ;
  //  m_nodeXslt = new QLineEdit ;
  m_clean = new QLineEdit ;
  m_find = new QLineEdit ;
  m_findNext = new QLineEdit ;
  m_forward = new QLineEdit ;
  m_home = new QLineEdit ;
  m_margin = new QLineEdit ;
  m_margin->setValidator(new QIntValidator);
  m_mark = new QLineEdit ;
  m_focusUp = new QLineEdit ;
  m_focusDown = new QLineEdit ;
  m_narrow = new QLineEdit ;
  m_reload = new QLineEdit ;
  m_offPage = new QCheckBox;
  // the next four are not shown
  // these need to have a parent otherwise findChildren routines in OptionsWidget wont pick them up
  m_saveHtml = new QCheckBox(this) ;
  m_saveXml = new QCheckBox(this) ;
  m_saveOutputHtml = new QCheckBox(this) ;
  m_outputPath = new QLineEdit(this);

  m_saveOutputHtml->setVisible(false);
  m_saveHtml->setVisible(false);
  m_saveXml->setVisible(false);
  m_outputPath->setVisible(false);

  m_show = new QLineEdit ;
  m_showLinkWarning = new QCheckBox ;
  m_widenStep = new QLineEdit ;
  m_widenStep->setValidator(new QIntValidator);
  m_scaleStep = new QLineEdit ;
  m_scaleStep->setValidator(new QIntValidator);
  m_highlightColor = new QLineEdit ;
  m_textWidth = new QLineEdit ;
  m_textWidth->setValidator(new QIntValidator);

  m_widen = new QLineEdit ;
  m_zoom = new QDoubleSpinBox ;
  this->setControlSize(m_zoom,75);
  m_zoom->setDecimals(2);
  m_zoom->setSingleStep(0.01);
  m_zoomIn = new QLineEdit ;
  this->setControlSize(m_zoomIn,SMALL_EDIT);

  m_zoomOut = new QLineEdit ;
  this->setControlSize(m_zoomOut,SMALL_EDIT);
  QVBoxLayout * vlayout = new QVBoxLayout;

  QGroupBox * keybox = new QGroupBox(tr("Keyboard commands"));
  //  QFormLayout * keylayout = new QFormLayout;
  QGridLayout * keylayout = new QGridLayout;

  /// Zoom
  keylayout->addWidget(new QLabel(tr("Zoom out")),0,0);
  keylayout->addWidget(m_zoomOut,0,1);

  keylayout->addWidget(new QLabel(tr("Zoom in")),1,0);
  keylayout->addWidget(m_zoomIn,1,1);


  /// Focus
  this->setControlSize(m_home,SMALL_EDIT);
  this->setControlSize(m_mark,SMALL_EDIT);
  this->setControlSize(m_focusUp,SMALL_EDIT);
  this->setControlSize(m_focusDown,SMALL_EDIT);
  keylayout->addWidget(new QLabel(tr("Move up")),2,0);
  keylayout->addWidget(m_focusUp,2,1);
  keylayout->addWidget(new QLabel(tr("Move down")),3,0);
  keylayout->addWidget(m_focusDown,3,1);
  keylayout->addWidget(new QLabel(tr("Home")),4,0);
  keylayout->addWidget(m_home,4,1);
  keylayout->addWidget(new QLabel(tr("Mark current entry as home")),5,0);
  keylayout->addWidget(m_mark,5,1);

  /// Text size
  this->setControlSize(m_textWidth,MEDIUM_EDIT);
  this->setControlSize(m_margin,MEDIUM_EDIT);
  this->setControlSize(m_narrow,SMALL_EDIT);
  this->setControlSize(m_widen,SMALL_EDIT);
  this->setControlSize(m_widenStep,SMALL_EDIT);
  this->setControlSize(m_scaleStep,SMALL_EDIT);
  keylayout->addWidget(new QLabel(tr("Widen text")),6,0);
  keylayout->addWidget(m_widen,6,1);
  keylayout->addWidget(new QLabel("Narrow text"),7,0);
  keylayout->addWidget(m_narrow,7,1);

  // Lexicon movement
  this->setControlSize(m_back,SMALL_EDIT);
  this->setControlSize(m_forward,SMALL_EDIT);
  keylayout->addWidget(new QLabel(tr("Next root")),0,2);
  keylayout->addWidget(m_forward,0,3);
  keylayout->addWidget(new QLabel(tr("Previous root")),1,2);
  keylayout->addWidget(m_back,1,3);



  /// Local search

  this->setControlSize(m_find,SMALL_EDIT);
  this->setControlSize(m_findNext,SMALL_EDIT);
  this->setControlSize(m_clean,SMALL_EDIT);
  keylayout->addWidget(new QLabel(tr("Find")),2,2);
  keylayout->addWidget(m_find,2,3);
  keylayout->addWidget(new QLabel(tr("Find next")),3,2);
  keylayout->addWidget(m_findNext,3,3);
  keylayout->addWidget(new QLabel(tr("Clear highlight")),4,2);
  keylayout->addWidget(m_clean,4,3);
  keylayout->addWidget(new QLabel(tr("Show last search results")),5,2);
  keylayout->addWidget(m_show,5,3);
  this->setControlSize(m_reload,SMALL_EDIT);
  keylayout->addWidget(new QLabel(tr("Reload page")),6,2);
  keylayout->addWidget(m_reload,6,3);

  keylayout->setHorizontalSpacing(100);
  keybox->setLayout(keylayout);

  QGroupBox * cssbox = new QGroupBox(tr("XSLT/CSS"));
  QFormLayout * csslayout = new QFormLayout;
  csslayout->addRow(tr("Stylesheet"),m_css);
  csslayout->addRow(tr("Print stylesheet"),m_printCss);
  csslayout->addRow(tr("XSLT"),m_entryXslt);
  //  csslayout->addRow(tr("Node XSLT"),m_nodeXslt);
  cssbox->setLayout(csslayout);

  QGroupBox * otherbox = new QGroupBox(tr("Other"));
  QFormLayout * otherlayout = new QFormLayout;

  /// Assorted


  this->setControlSize(m_show,SMALL_EDIT);
  this->setControlSize(m_showLinkWarning,SMALL_EDIT);
  this->setControlSize(m_highlightColor,LARGE_EDIT);
  otherlayout->addRow(tr("Off page movement"),m_offPage);

  QHBoxLayout * zoomlayout = new QHBoxLayout;
  zoomlayout->addWidget(m_zoom);
  zoomlayout->addSpacing(20);
  zoomlayout->addWidget(new QLabel(tr("Zoom step size")));
  zoomlayout->addSpacing(10);
  zoomlayout->addWidget(m_scaleStep);
  zoomlayout->addStretch();

  otherlayout->addRow(tr("Default zoom"),zoomlayout);

  //  otherlayout->addRow(tr("Default zoom"),m_zoom);
  //  otherlayout->addRow(tr("Zoom step size"),m_scaleStep);
  otherlayout->addRow(tr("Show link warning"),m_showLinkWarning);

  QHBoxLayout * widthlayout = new QHBoxLayout;
  widthlayout->addWidget(m_textWidth);
  widthlayout->addSpacing(20);
  widthlayout->addWidget(new QLabel(tr("Width step size")));
  widthlayout->addSpacing(10);
  widthlayout->addWidget(m_widenStep);
  widthlayout->addStretch();

  otherlayout->addRow(tr("Text width"),widthlayout);

  //  otherlayout->addRow(tr("Widen/narrow step size"),m_widenStep);
  //  otherlayout->addRow(tr("Text width"),m_textWidth);


  otherlayout->addRow(tr("Text margin"),m_margin);
  otherlayout->addRow(tr("Close entry info after load"),m_nodeinfoClose);

  QHBoxLayout * colorlayout = new QHBoxLayout;
  colorlayout->addWidget(m_highlightColor);
  QPushButton * colorbutton = new QPushButton(tr("Set colour"));
  connect(colorbutton,SIGNAL(clicked()),this,SLOT(onSetColor()));
  colorlayout->addWidget(colorbutton);
  colorlayout->addStretch();
  otherlayout->addRow(tr("Highlight color"),colorlayout);

  QPushButton * advancedbutton = new QPushButton(tr("Options"));
  connect(advancedbutton,SIGNAL(clicked()),this,SLOT(onAdvanced()));

  QHBoxLayout * adlayout = new QHBoxLayout;
  adlayout->addWidget(advancedbutton);
  adlayout->addStretch();
  otherlayout->addRow(tr("Advanced"),adlayout);


  otherbox->setLayout(otherlayout);

  vlayout->addWidget(keybox);
  //  vlayout->addWidget(printbox);
  vlayout->addWidget(cssbox);
  vlayout->addWidget(otherbox);
  vlayout->addStretch();

  setLayout(vlayout);
  addButtons();
  readSettings();
  setupConnections();
  this->setComboSize(VLARGE_EDIT);
  //  getLexicon()->setCursorPosition(this);
}

void EntryOptions::readSettings(bool /* reload */) {
  QString v;
    QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");



  settings.beginGroup("XSLT");
  v = settings.value(SID_XSLT_ENTRY,QString("entry.xslt")).toString();
  //  v = getLexicon()->getResourceFilePath(Lexicon::XSLT,v);
  //  v = QDir::current().relativeFilePath(v);
  m_entryXslt->setText(v);

  //  v = settings.value(SID_XSLT_NODE).toString();
  //  v = getLexicon()->getResourceFilePath(Lexicon::XSLT,v);
  //  v = QDir::current().relativeFilePath(v);
  //  m_nodeXslt->setText(v);
  settings.endGroup();
  settings.beginGroup(m_section);
  m_css->setText(settings.value(SID_ENTRY_CSS).toString());
  m_printCss->setText(settings.value(SID_ENTRY_PRINT_CSS).toString());

  m_nodeinfoClose->setChecked(settings.value(SID_ENTRY_NODEINFO_CLOSE).toBool());

  m_back->setText(settings.value(SID_ENTRY_BACK).toString());
  m_clean->setText(settings.value(SID_ENTRY_CLEAN).toString());
  m_offPage->setChecked(settings.value(SID_ENTRY_OFF_PAGE).toBool());
  m_find->setText(settings.value(SID_ENTRY_FIND).toString());
  m_findNext->setText(settings.value(SID_ENTRY_FIND_NEXT).toString());
  m_forward->setText(settings.value(SID_ENTRY_FORWARD).toString());
  m_home->setText(settings.value(SID_ENTRY_HOME).toString());
  m_margin->setText(settings.value(SID_ENTRY_MARGIN).toString());
  m_mark->setText(settings.value(SID_ENTRY_MARK).toString());
  m_focusUp->setText(settings.value(SID_ENTRY_MOVE_FOCUS_UP).toString());
  m_focusDown->setText(settings.value(SID_ENTRY_MOVE_FOCUS_DOWN).toString());
  m_narrow->setText(settings.value(SID_ENTRY_NARROW).toString());
  m_reload->setText(settings.value(SID_ENTRY_RELOAD).toString());
  m_saveHtml->setChecked(settings.value(SID_ENTRY_DUMP_HTML).toBool());
  m_saveXml->setChecked(settings.value(SID_ENTRY_DUMP_XML).toBool());
  m_saveOutputHtml->setChecked(settings.value(SID_ENTRY_DUMP_OUTPUT_HTML).toBool());
  m_outputPath->setText(settings.value(SID_ENTRY_OUTPUT_PATH,QDir::tempPath()).toString());
  m_show->setText(settings.value(SID_ENTRY_SHOW).toString());
  m_showLinkWarning->setChecked(settings.value(SID_ENTRY_SHOW_LINK_WARNING).toBool());
  m_widenStep->setText(settings.value(SID_ENTRY_WIDEN_STEP).toString());
  m_scaleStep->setText(settings.value(SID_ENTRY_SCALE_STEP).toString());
  m_highlightColor->setText(settings.value(SID_ENTRY_HIGHLIGHT_COLOR).toString());
  m_textWidth->setText(settings.value(SID_ENTRY_TEXT_WIDTH).toString());
  m_widen->setText(settings.value(SID_ENTRY_WIDEN).toString());
  m_zoom->setValue(settings.value(SID_ENTRY_SCALE).toDouble());
  m_zoomIn->setText(settings.value(SID_ENTRY_ZOOM_IN).toString());
  m_zoomOut->setText(settings.value(SID_ENTRY_ZOOM_OUT).toString());
  m_dirty = false;
}
void EntryOptions::writeSettings(const QString & fileName) {
  QString f = m_settingsFileName;
  if (!fileName.isEmpty()) {
    f = fileName;
  }
  QSettings settings(f,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  settings.setValue(SID_ENTRY_NODEINFO_CLOSE,m_nodeinfoClose->isChecked());
  settings.setValue(SID_ENTRY_BACK,m_back->text());
  settings.setValue(SID_ENTRY_CSS,m_css->text());
  settings.setValue(SID_ENTRY_PRINT_CSS,m_printCss->text());
  settings.setValue(SID_ENTRY_CLEAN,m_clean->text());
  settings.setValue(SID_ENTRY_FIND,m_find->text());
  settings.setValue(SID_ENTRY_FIND_NEXT,m_findNext->text());
  settings.setValue(SID_ENTRY_FORWARD,m_forward->text());
  settings.setValue(SID_ENTRY_HOME,m_home->text());
  settings.setValue(SID_ENTRY_MARGIN,m_margin->text());
  settings.setValue(SID_ENTRY_MARK,m_mark->text());
  settings.setValue(SID_ENTRY_MOVE_FOCUS_UP,m_focusUp->text());
  settings.setValue(SID_ENTRY_MOVE_FOCUS_DOWN,m_focusDown->text());
  settings.setValue(SID_ENTRY_NARROW,m_narrow->text());
  settings.setValue(SID_ENTRY_RELOAD,m_reload->text());
  settings.setValue(SID_ENTRY_DUMP_HTML,m_saveHtml->isChecked());
  settings.setValue(SID_ENTRY_DUMP_XML,m_saveXml->isChecked());
  settings.setValue(SID_ENTRY_DUMP_OUTPUT_HTML,m_saveOutputHtml->isChecked());
  settings.setValue(SID_ENTRY_OUTPUT_PATH,m_outputPath->text());
  settings.setValue(SID_ENTRY_SHOW,m_show->text());
  settings.setValue(SID_ENTRY_SHOW_LINK_WARNING,m_showLinkWarning->isChecked());

  settings.setValue(SID_ENTRY_WIDEN_STEP,m_widenStep->text());
  settings.setValue(SID_ENTRY_SCALE_STEP,m_scaleStep->text());

  settings.setValue(SID_ENTRY_HIGHLIGHT_COLOR,m_highlightColor->text());

  settings.setValue(SID_ENTRY_TEXT_WIDTH,m_textWidth->text());
  settings.setValue(SID_ENTRY_WIDEN,m_widen->text());
  settings.setValue(SID_ENTRY_SCALE,m_zoom->value());
  settings.setValue(SID_ENTRY_ZOOM_IN,m_zoomIn->text());
  settings.setValue(SID_ENTRY_ZOOM_OUT,m_zoomOut->text());
  settings.setValue(SID_ENTRY_OFF_PAGE,m_offPage->isChecked());

  settings.endGroup();
  settings.beginGroup("XSLT");
  settings.setValue(SID_XSLT_ENTRY,m_entryXslt->text());
  //  settings.setValue(SID_XSLT_NODE,m_nodeXslt->text());

  settings.sync();
  m_dirty = false;
  emit(modified(false));
}
/**
 *
 *
 *
 * @return
 */
bool EntryOptions::isModified()  {
  m_dirty = false;
  m_changes.clear();

  QSettings settings(m_settingsFileName,QSettings::IniFormat);
  settings.setIniCodec("UTF-8");
  settings.beginGroup(m_section);

  if (compare(&settings,SID_ENTRY_NODEINFO_CLOSE,m_nodeinfoClose)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_BACK,m_back)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_CSS,m_css)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_PRINT_CSS,m_printCss)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_CLEAN,m_clean)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_FIND,m_find)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_FIND_NEXT,m_findNext)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_FORWARD,m_forward)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_HOME,m_home)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_MARGIN,m_margin)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_MARK,m_mark)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_MOVE_FOCUS_UP,m_focusUp)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_MOVE_FOCUS_DOWN,m_focusDown)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_NARROW,m_narrow)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_RELOAD,m_reload)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_DUMP_HTML,m_saveHtml)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_DUMP_XML,m_saveXml)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_DUMP_OUTPUT_HTML,m_saveOutputHtml)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_OUTPUT_PATH,m_outputPath)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_SHOW,m_show)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_SHOW_LINK_WARNING,m_showLinkWarning)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_WIDEN_STEP,m_widenStep)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_SCALE_STEP,m_scaleStep)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_HIGHLIGHT_COLOR,m_highlightColor)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_TEXT_WIDTH,m_textWidth)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_WIDEN,m_widen)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_SCALE,m_zoom)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_ZOOM_IN,m_zoomIn)) {
    m_dirty = true;
  }

  if (compare(&settings,SID_ENTRY_ZOOM_OUT,m_zoomOut)) {
    m_dirty = true;
  }
  if (compare(&settings,SID_ENTRY_OFF_PAGE,m_offPage)) {
    m_dirty = true;
  }
  settings.endGroup();
  settings.beginGroup("XSLT");
  if (compare(&settings,SID_XSLT_ENTRY,m_entryXslt)) {
    m_dirty = true;
  }
  /*
  if (compare(&settings,SID_XSLT_NODE,m_nodeXslt)) {
    m_dirty = true;
  }
  */

  return m_dirty;
}
void EntryOptions::onSetFont() {
  /*
  bool arabic = false;
  QPushButton * button = qobject_cast<QPushButton *>(QObject::sender());
  QFont f;
  if (button == m_standardButton) {
    f.fromString(m_standardFont->text());
  }
  else {
    arabic = true;
    f.fromString(m_arabicFont->text());
  }
  if (arabic) {
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
      m_arabicFont->setText(font.toString());
    }
    delete d;
    return;
  }
  bool ok;
  QFont font = QFontDialog::getFont(&ok, f,  this);
  if (!ok) {
    return;
  }
  m_standardFont->setText(font.toString());
  */
}
void EntryOptions::onSetColor() {
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
}
void EntryOptions::onAdvanced() {
  DebugOptionsDialog d(
                       m_saveXml->isChecked(),
                       m_saveHtml->isChecked(),
                       m_saveOutputHtml->isChecked(),
                       m_outputPath->text()
);

  if (d.exec() == QDialog::Accepted) {
    this->m_saveHtml->setChecked(d.m_html->isChecked());
    this->m_saveXml->setChecked(d.m_xml->isChecked());
    this->m_saveOutputHtml->setChecked(d.m_outputHtml->isChecked());
    this->m_outputPath->setText(d.m_path->text());
  }
}
